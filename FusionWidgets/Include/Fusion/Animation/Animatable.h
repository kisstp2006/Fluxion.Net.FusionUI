#pragma once

namespace Fusion
{
    class FWidget;

    template<class T>
    concept FWidgetClassType = TFIsDerivedClass<FWidget, T>::Value;

    template<typename T>
    struct FAnimatable
    {
        static constexpr bool Supported = requires (T a, T b) { a + b; };

        static T Lerp(const T& a, const T& b, f32 t) { return a + (b - a) * FMath::Clamp01(t); }

        static T LerpUnclamped(const T& a, const T& b, f32 t) { return a + (b - a) * t; }

        // Identity/zero value — needed for spring initial velocity
        static T Identity() { return {}; }

        // Spring arithmetic helpers
        static T Add(const T& a, const T& b) { return a + b; }
        static T Scale(const T& a, f32 s) { return a * s; }

        static f32 SquaredMagnitude(const T& v)
        {
            if constexpr (requires { v.GetSqrMagnitude(); })
                return v.GetSqrMagnitude();
            else
                return static_cast<f32>(v * v);
        }
    };

    template<>
    struct FAnimatable<FColor>
    {
        static constexpr bool Supported = true;

        static FColor Lerp(const FColor& a, const FColor& b, f32 t) { return FColor::Lerp(a, b, FMath::Clamp01(t)); }

        static FColor LerpUnclamped(const FColor& a, const FColor& b, f32 t) { return FColor::Lerp(a, b, t); }

        // Identity/zero value — needed for spring initial velocity
        static FColor Identity() { return {}; }

        // Spring arithmetic helpers
        static FColor Add(const FColor& a, const FColor& b)
        {
            return FColor(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
        }

        static FColor Scale(const FColor& a, f32 s) { return a * s; }

        static f32 SquaredMagnitude(const FColor& v)
        {
            return v.r * v.r + v.g * v.g + v.b * v.b + v.a * v.a;
        }
    };

    template<>
    struct FAnimatable<FAffineTransform>
    {
        static constexpr bool Supported = true;

        static FAffineTransform Lerp(const FAffineTransform& a, const FAffineTransform& b, f32 t)
        {
            return LerpUnclamped(a, b, FMath::Clamp01(t));
        }

        static FAffineTransform LerpUnclamped(const FAffineTransform& a, const FAffineTransform& b, f32 t)
        {
            ZoneScoped;

            // Decompose a
            const f32 sxA = FMath::Sqrt(a.m00 * a.m00 + a.m10 * a.m10);
            const f32 thetaA = std::atan2(a.m10, a.m00);
            const f32 cosA = sxA > 1e-8f ? a.m00 / sxA : 1.0f;
            const f32 sinA = sxA > 1e-8f ? a.m10 / sxA : 0.0f;
            const f32 shearA = cosA * a.m01 + sinA * a.m11;
            const f32 syA = -sinA * a.m01 + cosA * a.m11;

            // Decompose b
            const f32 sxB = FMath::Sqrt(b.m00 * b.m00 + b.m10 * b.m10);
            const f32 thetaB = std::atan2(b.m10, b.m00);
            const f32 cosB = sxB > 1e-8f ? b.m00 / sxB : 1.0f;
            const f32 sinB = sxB > 1e-8f ? b.m10 / sxB : 0.0f;
            const f32 shearB = cosB * b.m01 + sinB * b.m11;
            const f32 syB = -sinB * b.m01 + cosB * b.m11;

            // Lerp each component
            const f32 tx = a.tx + (b.tx - a.tx) * t;
            const f32 ty = a.ty + (b.ty - a.ty) * t;
            const f32 sx = sxA + (sxB - sxA) * t;
            const f32 sy = syA + (syB - syA) * t;
            const f32 theta = thetaA + (thetaB - thetaA) * t;
            const f32 shear = shearA + (shearB - shearA) * t;

            // Recompose
            const f32 cosT = std::cos(theta);
            const f32 sinT = std::sin(theta);

            FAffineTransform out;
            out.m00 = cosT * sx;
            out.m10 = sinT * sx;
            out.m01 = cosT * shear - sinT * sy;
            out.m11 = sinT * shear + cosT * sy;
            out.tx = tx;
            out.ty = ty;
            return out;
        }

        // Identity/zero value — needed for spring initial velocity
        static FAffineTransform Identity() { return FAffineTransform::Identity(); }

        // Spring arithmetic helpers — element-wise, treating the matrix as a 6-vector
        static FAffineTransform Add(const FAffineTransform& a, const FAffineTransform& b)
        {
            FAffineTransform out;
            out.m00 = a.m00 + b.m00; out.m01 = a.m01 + b.m01;
            out.m10 = a.m10 + b.m10; out.m11 = a.m11 + b.m11;
            out.tx = a.tx + b.tx;  out.ty = a.ty + b.ty;
            return out;
        }

        static FAffineTransform Scale(const FAffineTransform& a, f32 s)
        {
            FAffineTransform out;
            out.m00 = a.m00 * s; out.m01 = a.m01 * s;
            out.m10 = a.m10 * s; out.m11 = a.m11 * s;
            out.tx = a.tx * s; out.ty = a.ty * s;
            return out;
        }

        static f32 SquaredMagnitude(const FAffineTransform& v)
        {
            return v.m00 * v.m00 + v.m01 * v.m01 + v.m10 * v.m10 + v.m11 * v.m11 + v.tx * v.tx + v.ty * v.ty;
        }
    };

    template<>
    struct FAnimatable<FGradient>
    {
        static constexpr bool Supported = true;

        // Sample a gradient's color at a normalized Position by interpolating between adjacent GetStops().
        static FColor Sample(const FGradient& g, f32 t)
        {
            ZoneScoped;

            if (g.GetStops().Empty())                   return FColor(0, 0, 0, 0);
            if (t <= g.GetStops()[0].Position)             return g.GetStops()[0].Color;
            if (t >= g.GetStops().Last().Position)         return g.GetStops().Last().Color;

            for (int i = 0; i < (int)g.GetStops().Size() - 1; i++)
            {
                if (t >= g.GetStops()[i].Position && t <= g.GetStops()[i + 1].Position)
                {
                    const f32 range = g.GetStops()[i + 1].Position - g.GetStops()[i].Position;
                    const f32 localT = range > 1e-8f ? (t - g.GetStops()[i].Position) / range : 0.f;
                    return FColor::Lerp(g.GetStops()[i].Color, g.GetStops()[i + 1].Color, localT);
                }
            }
            return g.GetStops().Last().Color;
        }

        static FGradient Lerp(const FGradient& a, const FGradient& b, f32 t)
        {
            return LerpUnclamped(a, b, FMath::Clamp01(t));
        }

        static FGradient LerpUnclamped(const FGradient& a, const FGradient& b, f32 t)
        {
            ZoneScoped;

            // Fast path: same type and same stop count — lerp per-stop directly
            if (a.GetType() == b.GetType() && a.GetStops().Size() == b.GetStops().Size())
            {
                FGradient out = a;
                for (int i = 0; i < (int)a.GetStops().Size(); i++)
                {
                    out.GetStops()[i].Color = FColor::Lerp(a.GetStops()[i].Color, b.GetStops()[i].Color, t);
                    out.GetStops()[i].Position = a.GetStops()[i].Position + (b.GetStops()[i].Position - a.GetStops()[i].Position) * t;
                }
                out.SetAngle(a.GetAngle() + (b.GetAngle() - a.GetAngle()) * t);
                return out;
            }

            // Different stop counts: merge the two sorted Position lists into a union,
            // sample each gradient at every Position, then lerp the sampled colors.
            FGradient out;
            out.SetType(t < 0.5f ? a.GetType() : b.GetType());
            out.SetAngle(a.GetAngle() + (b.GetAngle() - a.GetAngle()) * t);

            int ai = 0, bi = 0;
            const int na = (int)a.GetStops().Size();
            const int nb = (int)b.GetStops().Size();

            while (ai < na || bi < nb)
            {
                const f32 pa = ai < na ? a.GetStops()[ai].Position : 2.f;
                const f32 pb = bi < nb ? b.GetStops()[bi].Position : 2.f;

                f32 pos;
                if (pa <= pb) { pos = pa; ai++; if (FMath::Abs(pa - pb) < 1e-6f) bi++; }
                else { pos = pb; bi++; }

                // Skip positions that are effectively duplicates of the last one added
                if (!out.GetStops().Empty() && FMath::Abs(out.GetStops().Last().Position - pos) < 1e-6f)
                    continue;

                out.GetStops().Add(FGradientStop{ FColor::Lerp(Sample(a, pos), Sample(b, pos), t), pos });
            }

            return out;
        }

        // Identity/zero value — empty gradient acts as additive zero for spring arithmetic
        static FGradient Identity() { return FGradient{}; }

        static FGradient Add(const FGradient& a, const FGradient& b)
        {
            ZoneScoped;

            // Empty gradient acts as additive zero
            if (b.GetStops().Empty()) return a;
            if (a.GetStops().Empty()) return b;

            // Fast path: same stop count
            if (a.GetStops().Size() == b.GetStops().Size())
            {
                FGradient out = a;
                for (int i = 0; i < (int)a.GetStops().Size(); i++)
                {
                    out.GetStops()[i].Color = FAnimatable<FColor>::Add(a.GetStops()[i].Color, b.GetStops()[i].Color);
                    out.GetStops()[i].Position = a.GetStops()[i].Position + b.GetStops()[i].Position;
                }
                out.SetAngle(a.GetAngle() + b.GetAngle());
                return out;
            }

            // Different counts: union merge
            FGradient out;
            out.SetType(a.GetType());
            out.SetAngle(a.GetAngle() + b.GetAngle());

            int ai = 0, bi = 0;
            const int na = (int)a.GetStops().Size();
            const int nb = (int)b.GetStops().Size();

            while (ai < na || bi < nb)
            {
                const f32 pa = ai < na ? a.GetStops()[ai].Position : 2.f;
                const f32 pb = bi < nb ? b.GetStops()[bi].Position : 2.f;

                f32 pos;
                if (pa <= pb) { pos = pa; ai++; if (FMath::Abs(pa - pb) < 1e-6f) bi++; }
                else { pos = pb; bi++; }

                if (!out.GetStops().Empty() && FMath::Abs(out.GetStops().Last().Position - pos) < 1e-6f)
                    continue;

                out.GetStops().Add(FGradientStop{ FAnimatable<FColor>::Add(Sample(a, pos), Sample(b, pos)), pos });
            }

            return out;
        }

        static FGradient Scale(const FGradient& a, f32 s)
        {
            ZoneScoped;

            FGradient out = a;
            for (int i = 0; i < (int)a.GetStops().Size(); i++)
            {
                out.GetStops()[i].Color = FAnimatable<FColor>::Scale(a.GetStops()[i].Color, s);
                out.GetStops()[i].Position = a.GetStops()[i].Position * s;
            }
            out.SetAngle(a.GetAngle() * s);
            return out;
        }

        static f32 SquaredMagnitude(const FGradient& v)
        {
            ZoneScoped;

            f32 mag = v.GetAngle() * v.GetAngle();
            for (int i = 0; i < (int)v.GetStops().Size(); i++)
                mag += FAnimatable<FColor>::SquaredMagnitude(v.GetStops()[i].Color);
            return mag;
        }
    };

    template<>
    struct FAnimatable<FPen>
    {
        static constexpr bool Supported = true;

        static FPen Lerp(const FPen& a, const FPen& b, f32 t)
        {
            return LerpUnclamped(a, b, FMath::Clamp01(t));
        }

        static FPen LerpUnclamped(const FPen& a, const FPen& b, f32 t)
        {
            ZoneScoped;

            // Scalar fields: lerp continuously regardless of style
            const f32 thickness = a.GetThickness() + (b.GetThickness() - a.GetThickness()) * t;
            const f32 dashLength = a.GetDashLength() + (b.GetDashLength() - a.GetDashLength()) * t;
            const f32 dashGap = a.GetDashGap() + (b.GetDashGap() - a.GetDashGap()) * t;
            const f32 gradientOffset = a.GetGradientOffset() + (b.GetGradientOffset() - a.GetGradientOffset()) * t;
            const f32 dashPhase = a.GetDashPhase() + (b.GetDashPhase() - a.GetDashPhase()) * t;

            // Style (Solid/Dashed/Dotted/None): discrete, snap at t=0.5 if different
            const EPenStyle style = (a.GetStyle() == b.GetStyle() || t < 0.5f) ? a.GetStyle() : b.GetStyle();

            // Color / gradient fill: mirror FBrush SolidFill<->Gradient logic
            FPen out = LerpColorFill(a, b, t);
            out.Style(style);
            out.Thickness(thickness);
            out.DashLength(dashLength);
            out.DashGap(dashGap);
            out.GradientOffset(gradientOffset);
            out.DashPhase(dashPhase);
            return out;
        }

        // Identity/zero value — needed for spring initial velocity
        static FPen Identity() { return FPen(); }

        // Spring arithmetic helpers
        static FPen Add(const FPen& a, const FPen& b)
        {
            ZoneScoped;

            // None acts as additive zero: adopt the other side's structure
            if (a.GetStyle() == EPenStyle::None && b.GetStyle() != EPenStyle::None)
            {
                FPen out = b;
                out.Color(FAnimatable<FColor>::Add(a.GetColor(), b.GetColor()));
                out.Thickness(a.GetThickness() + b.GetThickness());
                out.DashLength(a.GetDashLength() + b.GetDashLength());
                out.DashGap(a.GetDashGap() + b.GetDashGap());
                out.GradientOffset(a.GetGradientOffset() + b.GetGradientOffset());
                out.DashPhase(a.GetDashPhase() + b.GetDashPhase());
                return out;
            }

            // Build on a's structure
            FPen out = a;
            out.Thickness(a.GetThickness() + b.GetThickness());
            out.DashLength(a.GetDashLength() + b.GetDashLength());
            out.DashGap(a.GetDashGap() + b.GetDashGap());
            out.GradientOffset(a.GetGradientOffset() + b.GetGradientOffset());
            out.DashPhase(a.GetDashPhase() + b.GetDashPhase());
            out.Color(FAnimatable<FColor>::Add(a.GetColor(), b.GetColor()));

            if (a.HasGradient() && b.HasGradient())
            {
                out.Gradient(FAnimatable<FGradient>::Add(a.GetGradient(), b.GetGradient()));
            }

            return out;
        }

        static FPen Scale(const FPen& a, f32 s)
        {
            ZoneScoped;

            FPen out = a;
            out.Thickness(a.GetThickness() * s);
            out.DashLength(a.GetDashLength() * s);
            out.DashGap(a.GetDashGap() * s);
            out.GradientOffset(a.GetGradientOffset() * s);
            out.DashPhase(a.GetDashPhase() * s);
            out.Color(FAnimatable<FColor>::Scale(a.GetColor(), s));

            if (a.HasGradient())
            {
                out.Gradient(FAnimatable<FGradient>::Scale(a.GetGradient(), s));
            }

            return out;
        }

        static f32 SquaredMagnitude(const FPen& v)
        {
            ZoneScoped;

            f32 mag = FAnimatable<FColor>::SquaredMagnitude(v.GetColor())
                + v.GetThickness() * v.GetThickness()
                + v.GetDashLength() * v.GetDashLength()
                + v.GetDashGap() * v.GetDashGap()
                + v.GetGradientOffset() * v.GetGradientOffset()
                + v.GetDashPhase() * v.GetDashPhase();

            if (v.HasGradient())
            {
                const FGradient& g = v.GetGradient();
                for (int i = 0; i < (int)g.GetStops().Size(); i++)
                    mag += FAnimatable<FColor>::SquaredMagnitude(g.GetStops()[i].Color);
                mag += g.GetAngle() * g.GetAngle();
            }

            return mag;
        }

    private:

        // Interpolate only the color/gradient fill; thickness/dash/style set separately by LerpUnclamped
        static FPen LerpColorFill(const FPen& a, const FPen& b, f32 t)
        {
            ZoneScoped;

            const bool aHasGrad = a.HasGradient();
            const bool bHasGrad = b.HasGradient();

            if (aHasGrad && bHasGrad)
                return LerpGradientPen(a, b, t);

            if (!aHasGrad && !bHasGrad)
            {
                FPen out = a;
                out.Color(FColor::Lerp(a.GetColor(), b.GetColor(), t));
                return out;
            }

            // Solid color <-> Gradient: treat solid as a degenerate gradient
            if (!aHasGrad && bHasGrad)
                return LerpSolidGradientPen(a, b, t);         // t=0 → solid a, t=1 → gradient b
            else
                return LerpSolidGradientPen(b, a, 1.0f - t); // t=0 → gradient a, t=1 → solid b
        }

        // Both pens have gradients
        static FPen LerpGradientPen(const FPen& a, const FPen& b, f32 t)
        {
            ZoneScoped;

            FGradient outGrad = FAnimatable<FGradient>::LerpUnclamped(a.GetGradient(), b.GetGradient(), t);
            // Thickness is a placeholder; LerpUnclamped overwrites it
            return FPen::Gradient(outGrad, 1.0f, FColor::Lerp(a.GetColor(), b.GetColor(), t));
        }

        // solid: solid-color pen, gradient: gradient pen; t goes 0→1 solid→gradient
        static FPen LerpSolidGradientPen(const FPen& solid, const FPen& gradient, f32 t)
        {
            ZoneScoped;

            const FGradient& g = gradient.GetGradient();

            FGradient outGrad = g;
            for (int i = 0; i < (int)g.GetStops().Size(); i++)
                outGrad.GetStops()[i].Color = FColor::Lerp(solid.GetColor(), g.GetStops()[i].Color, t);
            outGrad.SetAngle(g.GetAngle() * t);

            // Tint lerps from White (no tint at t=0) to the gradient pen's tint
            return FPen::Gradient(outGrad, 1.0f, FColor::Lerp(FColors::White, gradient.GetColor(), t));
        }
    };

    template<>
    struct FAnimatable<FBrush>
    {
        static constexpr bool Supported = true;

        static FBrush Lerp(const FBrush& a, const FBrush& b, f32 t)
        {
            return LerpUnclamped(a, b, FMath::Clamp01(t));
        }

        static FBrush LerpUnclamped(const FBrush& a, const FBrush& b, f32 t)
        {
            ZoneScoped;

            const EBrushStyle styleA = a.GetBrushStyle();
            const EBrushStyle styleB = b.GetBrushStyle();

            if (styleA == styleB)
            {
                switch (styleA)
                {
                case EBrushStyle::SolidFill: return LerpSolidFill(a, b, t);
                case EBrushStyle::Gradient:  return LerpGradient(a, b, t);
                case EBrushStyle::Image:     return LerpImage(a, b, t);
                default:                     return t < 1.0f ? a : b;
                }
            }

            // SolidFill <-> Gradient: treat solid as a degenerate gradient
            if (styleA == EBrushStyle::SolidFill && styleB == EBrushStyle::Gradient)
                return LerpSolidGradient(a, b, t);
            if (styleA == EBrushStyle::Gradient && styleB == EBrushStyle::SolidFill)
                return LerpSolidGradient(b, a, 1.0f - t);

            // All other cross-style mismatches: snap at t=0.5
            return t < 0.5f ? a : b;
        }

        static FBrush Identity() { return FBrush(); }

        // Spring arithmetic helpers
        static FBrush Add(const FBrush& a, const FBrush& b)
        {
            ZoneScoped;

            const EBrushStyle styleA = a.GetBrushStyle();
            const EBrushStyle styleB = b.GetBrushStyle();

            // None acts as a zero element: adopt the other side's structure
            if (styleA == EBrushStyle::None && styleB != EBrushStyle::None)
            {
                FBrush out = b;
                out.Color(FAnimatable<FColor>::Add(a.GetColor(), b.GetColor()));
                return out;
            }

            if (styleA == EBrushStyle::Gradient && styleA == styleB)
            {
                FGradient outGrad = FAnimatable<FGradient>::Add(a.GetGradient(), b.GetGradient());
                return FBrush(outGrad, FAnimatable<FColor>::Add(a.GetColor(), b.GetColor()));
            }

            if (styleA == EBrushStyle::Image && styleA == styleB)
            {
                FBrush out = a;
                out.Color(FAnimatable<FColor>::Add(a.GetColor(), b.GetColor()));
                out.BrushSize(a.GetBrushSize() + b.GetBrushSize());
                out.BrushPosition(a.GetBrushPosition() + b.GetBrushPosition());
                return out;
            }

            // SolidFill and None (zero velocity) styles: add color only
            FBrush out = a;
            out.Color(FAnimatable<FColor>::Add(a.GetColor(), b.GetColor()));
            return out;
        }

        static FBrush Scale(const FBrush& a, f32 s)
        {
            ZoneScoped;

            const EBrushStyle style = a.GetBrushStyle();

            if (style == EBrushStyle::Gradient)
            {
                FGradient outGrad = FAnimatable<FGradient>::Scale(a.GetGradient(), s);
                return FBrush(outGrad, FAnimatable<FColor>::Scale(a.GetColor(), s));
            }

            if (style == EBrushStyle::Image)
            {
                FBrush out = a;
                out.Color(FAnimatable<FColor>::Scale(a.GetColor(), s));
                out.BrushSize(a.GetBrushSize() * s);
                out.BrushPosition(a.GetBrushPosition() * s);
                return out;
            }

            // SolidFill and None: scale color only
            FBrush out = a;
            out.Color(FAnimatable<FColor>::Scale(a.GetColor(), s));
            return out;
        }

        static f32 SquaredMagnitude(const FBrush& v)
        {
            ZoneScoped;

            f32 mag = FAnimatable<FColor>::SquaredMagnitude(v.GetColor());

            if (v.GetBrushStyle() == EBrushStyle::Gradient)
            {
                const FGradient& g = v.GetGradient();
                for (int i = 0; i < (int)g.GetStops().Size(); i++)
                    mag += FAnimatable<FColor>::SquaredMagnitude(g.GetStops()[i].Color);
                mag += g.GetAngle() * g.GetAngle();
            }

            return mag;
        }

    private:

        static FBrush LerpSolidFill(const FBrush& a, const FBrush& b, f32 t)
        {
            ZoneScoped;

            return FBrush::Solid(FColor::Lerp(a.GetColor(), b.GetColor(), t));
        }

        static FBrush LerpGradient(const FBrush& a, const FBrush& b, f32 t)
        {
            ZoneScoped;

            FGradient outGrad = FAnimatable<FGradient>::LerpUnclamped(a.GetGradient(), b.GetGradient(), t);
            return FBrush(outGrad, FColor::Lerp(a.GetColor(), b.GetColor(), t));
        }

        static FBrush LerpImage(const FBrush& a, const FBrush& b, f32 t)
        {
            ZoneScoped;

            const FName& imgA = a.GetImagePath();
            const FName& imgB = b.GetImagePath();

            // Snap image name at t=0.5 if different; discrete fields snap at t=1
            const FName& outImg = (imgA == imgB || t < 0.5f) ? imgA : imgB;
            FBrush out(outImg, FColor::Lerp(a.GetColor(), b.GetColor(), t));
            out.ImageFit(t < 1.0f ? a.GetImageFit() : b.GetImageFit());
            out.BrushTiling(t < 1.0f ? a.GetBrushTiling() : b.GetBrushTiling());
            out.SliceMargins(t < 1.0f ? a.GetSliceMargins() : b.GetSliceMargins());
            out.BrushSize(a.GetBrushSize() + (b.GetBrushSize() - a.GetBrushSize()) * t);
            out.BrushPosition(a.GetBrushPosition() + (b.GetBrushPosition() - a.GetBrushPosition()) * t);
            return out;
        }

        // solid is the SolidFill brush, gradient is the Gradient brush, t goes 0->1 solid->gradient
        static FBrush LerpSolidGradient(const FBrush& solid, const FBrush& gradient, f32 t)
        {
            ZoneScoped;

            const FGradient& g = gradient.GetGradient();

            FGradient outGrad = g;
            for (int i = 0; i < (int)g.GetStops().Size(); i++)
                outGrad.GetStops()[i].Color = FColor::Lerp(solid.GetColor(), g.GetStops()[i].Color, t);
            outGrad.SetAngle(g.GetAngle() * t);

            // Tint lerps from white (no tint at t=0) to the gradient's tint
            return FBrush(outGrad, FColor::Lerp(FColors::White, gradient.GetColor(), t));
        }
    };
    
} // namespace Fusion
