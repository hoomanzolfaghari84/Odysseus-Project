#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "Odysseus2D/Core/Application.hpp"

namespace Odysseus2D
{
    static sf::Color ToSFMLColor(b2HexColor color) {
        uint32_t hex = static_cast<uint32_t>(color);
        return sf::Color(
            static_cast<uint8_t>(hex >> 16),
            static_cast<uint8_t>(hex >> 8),
            static_cast<uint8_t>(hex),
            255
        );
    }

    static sf::Vector2f ToSFMLVec2(const b2Vec2& v, float scale = 1.f) {
        return { v.x * scale, v.y * scale };
    }

    static void SFMLDrawPolygon(const b2Vec2* verts, int count, b2HexColor color, void* ctx) {
        auto& win = *static_cast<sf::RenderWindow*>(ctx);
        sf::VertexArray lines{ sf::PrimitiveType::LineStrip, static_cast<std::size_t>(count + 1) };
        sf::Color col = ToSFMLColor(color);
        for (int i = 0; i < count; ++i) {
            lines[i].position = ToSFMLVec2(verts[i]);
            lines[i].color = col;
        }
        lines[count].position = ToSFMLVec2(verts[0]);
        lines[count].color = col;
        
        win.draw(lines);
    }

    static void SFMLDrawSolidPolygon(b2Transform xf, const b2Vec2* verts, int count,
        float radius, b2HexColor color, void* ctx) {
        auto& win = *static_cast<sf::RenderWindow*>(ctx);
        sf::ConvexShape poly;
        poly.setPointCount(count);
        for (int i = 0; i < count; ++i) {
            b2Vec2 v = b2TransformPoint(xf, verts[i]);
            poly.setPoint(i, ToSFMLVec2(v));
        }
        sf::Color fill = ToSFMLColor(color);
        fill.a = 100;
        poly.setFillColor(fill);
        poly.setOutlineColor(ToSFMLColor(color));
        poly.setOutlineThickness(1.f);
        win.draw(poly);
    }

    static void SFMLDrawCircle(b2Vec2 center, float radius, b2HexColor color, void* ctx) {
        auto& win = *static_cast<sf::RenderWindow*>(ctx);
        //float scale = 30.f;
        sf::CircleShape circle(radius);
        circle.setFillColor(ToSFMLColor(color));
        circle.setOutlineColor(ToSFMLColor(color));
        circle.setOutlineThickness(2.f);
        circle.setPosition(ToSFMLVec2(center) - sf::Vector2f(radius , radius ));
        win.draw(circle);
    }

    static void SFMLDrawSolidCircle(b2Transform xf, float radius, b2HexColor color, void* ctx) {
        auto& win = *static_cast<sf::RenderWindow*>(ctx);
        b2Vec2 center = xf.p;
        sf::CircleShape circle(radius);
        sf::Color fill = ToSFMLColor(color); //fill.a = 100;
        circle.setFillColor(fill);
        circle.setOutlineColor(ToSFMLColor(color));
        circle.setOutlineThickness(2.f);
        circle.setPosition(ToSFMLVec2(center) - sf::Vector2f(radius, radius));
        win.draw(circle);
    }

    static void SFMLDrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* ctx) {
        auto& win = *static_cast<sf::RenderWindow*>(ctx);
        sf::VertexArray line{ sf::PrimitiveType::Lines, 2 };
        line[0].position = ToSFMLVec2(p1);
        line[0].color = ToSFMLColor(color);
        line[1].position = ToSFMLVec2(p2);
        line[1].color = ToSFMLColor(color);
        win.draw(line);
    }

    static void SFMLDrawPoint(b2Vec2 p, float size, b2HexColor color, void* ctx) {
        auto& win = *static_cast<sf::RenderWindow*>(ctx);
        //float scale = 30.f;
        sf::CircleShape point(size);
        point.setFillColor(ToSFMLColor(color));
        point.setPosition(ToSFMLVec2(p) - sf::Vector2f(size, size));
        win.draw(point);
    }

    void CreateSFMLDebugDraw(b2DebugDraw& debugDraw) {
        debugDraw = b2DefaultDebugDraw();
        debugDraw.DrawPolygonFcn = SFMLDrawPolygon;
        debugDraw.DrawSolidPolygonFcn = SFMLDrawSolidPolygon;
        debugDraw.DrawCircleFcn = SFMLDrawCircle;
        debugDraw.DrawSolidCircleFcn = SFMLDrawSolidCircle;
        debugDraw.DrawSegmentFcn = SFMLDrawSegment;
        debugDraw.DrawPointFcn = SFMLDrawPoint;

        debugDraw.drawShapes = true;
        
        debugDraw.drawJoints = true;
        debugDraw.drawBounds = true;
        debugDraw.context = Application::Get().GetWindow(); // pass SFML render target
    }
}
