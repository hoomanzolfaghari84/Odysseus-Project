class RendererAPI
{
public:
    virtual ~RendererAPI() = default;

    virtual void Init() = 0;
    virtual void Clear(const glm::vec4& color) = 0;
    virtual void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation = 0.0f) = 0;
    virtual void DrawQuad(const glm::mat3& transform, const glm::vec4& color) = 0;
    
    
private:

};
