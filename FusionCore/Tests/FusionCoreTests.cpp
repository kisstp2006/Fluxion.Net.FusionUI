#include <gtest/gtest.h>

#include "Fusion/FusionCore.h"

#include <thread>
#include <vector>

using namespace Fusion;

#pragma region FString

TEST(FStringTest, DefaultConstructor)
{
    FString str;
    EXPECT_EQ(str.ByteLength(), 0);
    EXPECT_EQ(str.Length(), 0);
    EXPECT_TRUE(str.IsEmpty());
    EXPECT_STREQ(str.CStr(), "");
}

TEST(FStringTest, ConstructFromCStr)
{
    FString str("Hello");
    EXPECT_EQ(str.ByteLength(), 5);
    EXPECT_EQ(str.Length(), 5);
    EXPECT_FALSE(str.IsEmpty());
    EXPECT_STREQ(str.CStr(), "Hello");
}

TEST(FStringTest, ConstructFromNullCStr)
{
    FString str(nullptr);
    EXPECT_TRUE(str.IsEmpty());
    EXPECT_EQ(str.ByteLength(), 0);
    EXPECT_EQ(str.Length(), 0);
}

TEST(FStringTest, ConstructFromStringView)
{
    std::string_view sv = "Hello";
    FString str(sv);
    EXPECT_EQ(str.ByteLength(), 5);
    EXPECT_EQ(str.Length(), 5);
    EXPECT_STREQ(str.CStr(), "Hello");
}

TEST(FStringTest, ConstructFromStdString)
{
    std::string s = "Hello";
    FString str(s);
    EXPECT_EQ(str.ByteLength(), 5);
    EXPECT_EQ(str.Length(), 5);
    EXPECT_STREQ(str.CStr(), "Hello");
}

TEST(FStringTest, CopyConstructor)
{
    FString a("Hello");
    FString b(a);
    EXPECT_EQ(b.ByteLength(), a.ByteLength());
    EXPECT_EQ(b.Length(), a.Length());
    EXPECT_STREQ(b.CStr(), "Hello");
}

TEST(FStringTest, MoveConstructor)
{
    FString a("Hello");
    FString b(std::move(a));
    EXPECT_STREQ(b.CStr(), "Hello");
    EXPECT_TRUE(a.IsEmpty());
}

TEST(FStringTest, CopyAssignment)
{
    FString a("Hello");
    FString b;
    b = a;
    EXPECT_STREQ(b.CStr(), "Hello");
    EXPECT_STREQ(a.CStr(), "Hello"); // a unchanged
}

TEST(FStringTest, MoveAssignment)
{
    FString a("Hello");
    FString b;
    b = std::move(a);
    EXPECT_STREQ(b.CStr(), "Hello");
    EXPECT_TRUE(a.IsEmpty());
}

TEST(FStringTest, AssignFromCStr)
{
    FString str;
    str = "World";
    EXPECT_STREQ(str.CStr(), "World");
    EXPECT_EQ(str.ByteLength(), 5);
}

TEST(FStringTest, AssignFromStringView)
{
    FString str;
    str = std::string_view("World");
    EXPECT_STREQ(str.CStr(), "World");
}

// SSO boundary: 15 bytes inline, 16+ goes to heap
TEST(FStringTest, SSOBoundaryInline)
{
    FString str("123456789012345"); // exactly 15 bytes
    EXPECT_EQ(str.ByteLength(), 15);
    EXPECT_EQ(str.Length(), 15);
    EXPECT_STREQ(str.CStr(), "123456789012345");
}

TEST(FStringTest, SSOBoundaryHeap)
{
    FString str("1234567890123456"); // 16 bytes, goes to heap
    EXPECT_EQ(str.ByteLength(), 16);
    EXPECT_EQ(str.Length(), 16);
    EXPECT_STREQ(str.CStr(), "1234567890123456");
}

TEST(FStringTest, AppendOperatorPlusEquals)
{
    FString str("Hello");
    str += " World";
    EXPECT_STREQ(str.CStr(), "Hello World");
    EXPECT_EQ(str.ByteLength(), 11);
    EXPECT_EQ(str.Length(), 11);
}

TEST(FStringTest, AppendOperatorPlus)
{
    FString a("Hello");
    FString b = a + " World";
    EXPECT_STREQ(b.CStr(), "Hello World");
    EXPECT_STREQ(a.CStr(), "Hello"); // a unchanged
}

TEST(FStringTest, AppendCausesSSoToHeapTransition)
{
    FString str("Hello!!");    // 7 bytes, SSO
    str += " This is a long string"; // pushes past 15 bytes, triggers heap
    EXPECT_STREQ(str.CStr(), "Hello!! This is a long string");
}

TEST(FStringTest, EqualityWithFString)
{
    FString a("Hello");
    FString b("Hello");
    FString c("World");
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}

TEST(FStringTest, EqualityWithCStr)
{
    FString str("Hello");
    EXPECT_EQ(str, "Hello");
    EXPECT_NE(str, "World");
}

TEST(FStringTest, EqualityWithStringView)
{
    FString str("Hello");
    EXPECT_EQ(str, std::string_view("Hello"));
    EXPECT_NE(str, std::string_view("World"));
}

TEST(FStringTest, View)
{
    FString str("Hello");
    std::string_view view = str.View();
    EXPECT_EQ(view, "Hello");
    EXPECT_EQ(view.size(), 5);
}

TEST(FStringTest, ToStdString)
{
    FString str("Hello");
    std::string s = str.ToStdString();
    EXPECT_EQ(s, "Hello");
}

// UTF-8: "café" — 4 codepoints, 5 bytes (é is 2 bytes in UTF-8)
TEST(FStringTest, UTF8ByteLengthVsLength)
{
    FString str("café");
    EXPECT_EQ(str.Length(), 4);     // 4 codepoints
    EXPECT_EQ(str.ByteLength(), 5); // 5 bytes
}

// UTF-8: "こんにちは" — 5 codepoints, 15 bytes (3 bytes each in UTF-8)
TEST(FStringTest, UTF8JapaneseSSoBoundary)
{
    FString str("こんにちは");
    EXPECT_EQ(str.Length(), 5);      // 5 codepoints
    EXPECT_EQ(str.ByteLength(), 15); // 15 bytes, exactly SSO limit
}

// UTF-8: "こんにちは!" — 6 codepoints, 16 bytes (heap)
TEST(FStringTest, UTF8JapaneseHeap)
{
    FString str("こんにちは!");
    EXPECT_EQ(str.Length(), 6);
    EXPECT_EQ(str.ByteLength(), 16);
}

TEST(FStringTest, UTF8AppendUpdatesCounts)
{
    FString str("café");
    str += " latte";
    EXPECT_EQ(str.Length(), 10);     // 4 + 6 codepoints
    EXPECT_EQ(str.ByteLength(), 11); // 5 + 6 bytes
}

#pragma endregion FString

#pragma region Ptr

TEST(PtrTest, ThreadSafety)
{
    class FTestObject : public FObject
    {
    public:
        std::atomic<int> m_Value = 0;
    };

    // Shared strong ref — all threads copy from this
    Ptr<FTestObject> shared(new FTestObject());

    constexpr int threadCount    = 8;
    constexpr int opsPerThread   = 10000;

    std::vector<std::thread> threads;
    threads.reserve(threadCount * 2);

    // Strong ref threads: repeatedly copy and release Ptr<T>
    for (int i = 0; i < threadCount; ++i)
    {
        threads.emplace_back([&]()
        {
            for (int j = 0; j < opsPerThread; ++j)
            {
                Ptr<FTestObject> local = shared;
                if (local)
                    local->m_Value.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    // Weak ref threads: repeatedly lock and release WeakPtr<T>
    WeakPtr<FTestObject> weak = shared;
    for (int i = 0; i < threadCount; ++i)
    {
        threads.emplace_back([&]()
        {
            for (int j = 0; j < opsPerThread; ++j)
            {
                Ptr<FTestObject> locked = weak.Lock();
                if (locked)
                    locked->m_Value.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    for (auto& t : threads)
        t.join();

    // Object should still be alive — shared still holds a strong ref
    EXPECT_TRUE(shared.IsValid());
    EXPECT_EQ(shared->m_Value.load(), threadCount * opsPerThread * 2);

    // Release shared — object should be destroyed, weak should be dead
    shared = nullptr;
    EXPECT_FALSE(weak.IsValid());
    EXPECT_EQ(weak.Lock(), nullptr);
}

#pragma endregion Ptr

#pragma region FArray

TEST(FArrayTest, DefaultConstructorEmpty)
{
    FArray<int> arr;
    EXPECT_EQ(arr.Size(), 0);
    EXPECT_TRUE(arr.IsEmpty());
    EXPECT_EQ(arr.Capacity(), 8); // default InlineCapacity
}

TEST(FArrayTest, InitializerList)
{
    FArray<int> arr = { 1, 2, 3 };
    EXPECT_EQ(arr.Size(), 3);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

TEST(FArrayTest, AddAndAccess)
{
    FArray<int> arr;
    arr.Add(10);
    arr.Add(20);
    arr.Add(30);
    EXPECT_EQ(arr.Size(), 3);
    EXPECT_EQ(arr.First(), 10);
    EXPECT_EQ(arr.Last(), 30);
}

TEST(FArrayTest, EmplaceReturnsRef)
{
    FArray<std::string> arr;
    std::string& ref = arr.Emplace("hello");
    EXPECT_EQ(ref, "hello");
    EXPECT_EQ(arr.Size(), 1);
}

TEST(FArrayTest, Pop)
{
    FArray<int> arr = { 1, 2, 3 };
    arr.Pop();
    EXPECT_EQ(arr.Size(), 2);
    EXPECT_EQ(arr.Last(), 2);
}

TEST(FArrayTest, PopEmpty)
{
    FArray<int> arr;
    arr.Pop(); // should not crash
    EXPECT_EQ(arr.Size(), 0);
}

TEST(FArrayTest, RemoveAtOrdered)
{
    FArray<int> arr = { 10, 20, 30, 40 };
    arr.RemoveAt(1); // remove 20
    EXPECT_EQ(arr.Size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 30);
    EXPECT_EQ(arr[2], 40);
}

TEST(FArrayTest, RemoveAtSwapUnordered)
{
    FArray<int> arr = { 10, 20, 30, 40 };
    arr.RemoveAtSwap(1); // swaps 20 with 40
    EXPECT_EQ(arr.Size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 40);
    EXPECT_EQ(arr[2], 30);
}

TEST(FArrayTest, RemoveAtSwapLastElement)
{
    FArray<int> arr = { 10, 20, 30 };
    arr.RemoveAtSwap(2); // removing last — no swap needed
    EXPECT_EQ(arr.Size(), 2);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
}

TEST(FArrayTest, Clear)
{
    FArray<int> arr = { 1, 2, 3 };
    arr.Clear();
    EXPECT_EQ(arr.Size(), 0);
    EXPECT_TRUE(arr.IsEmpty());
}

TEST(FArrayTest, ReserveDoesNotShrink)
{
    FArray<int> arr;
    arr.Reserve(20);
    EXPECT_GE(arr.Capacity(), 20);
    arr.Reserve(5);
    EXPECT_GE(arr.Capacity(), 20); // should not shrink
}

TEST(FArrayTest, ResizeDefaultValue)
{
    FArray<int> arr;
    arr.Resize(5);
    EXPECT_EQ(arr.Size(), 5);
    for (size_t i = 0; i < arr.Size(); ++i)
        EXPECT_EQ(arr[i], 0);
}

TEST(FArrayTest, ResizeWithValue)
{
    FArray<int> arr;
    arr.Resize(4, 42);
    EXPECT_EQ(arr.Size(), 4);
    for (size_t i = 0; i < arr.Size(); ++i)
        EXPECT_EQ(arr[i], 42);
}

TEST(FArrayTest, ResizeShrink)
{
    FArray<int> arr = { 1, 2, 3, 4, 5 };
    arr.Resize(3);
    EXPECT_EQ(arr.Size(), 3);
    EXPECT_EQ(arr[2], 3);
}

TEST(FArrayTest, Contains)
{
    FArray<int> arr = { 10, 20, 30 };
    EXPECT_TRUE(arr.Contains(20));
    EXPECT_FALSE(arr.Contains(99));
}

TEST(FArrayTest, IndexOf)
{
    FArray<int> arr = { 10, 20, 30 };
    EXPECT_EQ(arr.IndexOf(20), 1);
    EXPECT_EQ(arr.IndexOf(99), FArray<int>::npos);
}

// SBO boundary: inline capacity is 8, adding a 9th element triggers heap growth
TEST(FArrayTest, SBOInlineBoundary)
{
    FArray<int> arr;
    for (int i = 0; i < 8; ++i)
        arr.Add(i);
    EXPECT_EQ(arr.Size(), 8);
    EXPECT_EQ(arr.Capacity(), 8);

    arr.Add(8); // triggers grow
    EXPECT_EQ(arr.Size(), 9);
    EXPECT_GE(arr.Capacity(), 9);

    for (int i = 0; i < 9; ++i)
        EXPECT_EQ(arr[i], i);
}

TEST(FArrayTest, CopyConstructor)
{
    FArray<int> a = { 1, 2, 3 };
    FArray<int> b(a);
    EXPECT_EQ(b.Size(), 3);
    EXPECT_EQ(b[0], 1);
    b[0] = 99;
    EXPECT_EQ(a[0], 1); // deep copy — a unchanged
}

TEST(FArrayTest, MoveConstructorInline)
{
    FArray<int> a = { 1, 2, 3 };
    FArray<int> b(std::move(a));
    EXPECT_EQ(b.Size(), 3);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(a.Size(), 0); // a emptied
}

TEST(FArrayTest, MoveConstructorHeap)
{
    FArray<int> a;
    for (int i = 0; i < 16; ++i) // force heap
        a.Add(i);

    FArray<int> b(std::move(a));
    EXPECT_EQ(b.Size(), 16);
    EXPECT_EQ(b[0], 0);
    EXPECT_EQ(b[15], 15);
    EXPECT_EQ(a.Size(), 0);
}

TEST(FArrayTest, CopyAssignment)
{
    FArray<int> a = { 1, 2, 3 };
    FArray<int> b;
    b = a;
    EXPECT_EQ(b.Size(), 3);
    EXPECT_EQ(a.Size(), 3); // a unchanged
}

TEST(FArrayTest, MoveAssignment)
{
    FArray<int> a = { 1, 2, 3 };
    FArray<int> b;
    b = std::move(a);
    EXPECT_EQ(b.Size(), 3);
    EXPECT_EQ(a.Size(), 0);
}

TEST(FArrayTest, RangeFor)
{
    FArray<int> arr = { 1, 2, 3, 4 };
    int sum = 0;
    for (int v : arr)
        sum += v;
    EXPECT_EQ(sum, 10);
}

TEST(FArrayTest, CustomInlineCapacity)
{
    FArray<int, 2> arr;
    EXPECT_EQ(arr.Capacity(), 2);
    arr.Add(1);
    arr.Add(2);
    EXPECT_EQ(arr.Capacity(), 2);
    arr.Add(3); // goes to heap
    EXPECT_GE(arr.Capacity(), 3);
    EXPECT_EQ(arr.Size(), 3);
}

TEST(FArrayTest, ZeroInlineCapacity)
{
    FArray<int, 0> arr;
    EXPECT_EQ(arr.Capacity(), 0);
    arr.Add(1);
    EXPECT_EQ(arr.Size(), 1);
    EXPECT_EQ(arr[0], 1);
}

TEST(FArrayTest, NonTrivialType)
{
    FArray<FString> arr;
    arr.Add(FString("hello"));
    arr.Add(FString("world"));
    EXPECT_EQ(arr.Size(), 2);
    EXPECT_EQ(arr[0], "hello");
    EXPECT_EQ(arr[1], "world");

    arr.RemoveAt(0);
    EXPECT_EQ(arr.Size(), 1);
    EXPECT_EQ(arr[0], "world");
}

#pragma endregion FArray
