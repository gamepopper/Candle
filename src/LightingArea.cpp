#include "Candle/LightingArea.hpp"
#include "Candle/graphics/VertexArray.hpp"


namespace candle{
    
    sf::BlendMode l_substractAlpha(
        sf::BlendMode::Factor::Zero,              // color src
        sf::BlendMode::Factor::One,               // color dst
        sf::BlendMode::Equation::Add,             // color eq
        sf::BlendMode::Factor::Zero,              // alpha src
        sf::BlendMode::Factor::OneMinusSrcAlpha,  // alpha dst
        sf::BlendMode::Equation::Add    );            // alpha eq
    
    void LightingArea::initializeRenderTexture(const sf::Vector2f& size){
        m_renderTexture.resize(sf::Vector2u(size));
        m_renderTexture.setSmooth(true);
        m_baseTextureQuad[0].position =
        m_areaQuad[0].position =
        m_areaQuad[0].texCoords = {0, 0};
        m_baseTextureQuad[1].position =
        m_areaQuad[1].position =
        m_areaQuad[1].texCoords = {0, size.y};
        m_baseTextureQuad[2].position =
        m_areaQuad[2].position =
        m_areaQuad[2].texCoords = {size.x, 0};
        m_baseTextureQuad[3].position =
        m_areaQuad[3].position =
        m_areaQuad[3].texCoords = {size.x, size.y};
    }
    
    LightingArea::LightingArea(Mode mode, const sf::Vector2f& position, const sf::Vector2f& size)
    : m_baseTextureQuad(sf::PrimitiveType::TriangleStrip, 4)
    , m_areaQuad(sf::PrimitiveType::TriangleStrip, 4)
    , m_color(sf::Color::White)
    {
        m_opacity = 1.f;
        m_mode = mode;
        m_baseTexture = nullptr;
        initializeRenderTexture(size);
        Transformable::setPosition(position);
    }
    
    LightingArea::LightingArea(Mode mode, const sf::Texture* t, sf::IntRect r)
    : m_baseTextureQuad(sf::PrimitiveType::TriangleStrip, 4)
    , m_areaQuad(sf::PrimitiveType::TriangleStrip, 4)
    , m_color(sf::Color::White)
    {
        m_opacity = 1.f;
        m_mode = mode;
        setAreaTexture(t, r);
    }
    
    sf::FloatRect LightingArea::getLocalBounds () const{
        return m_areaQuad.getBounds();
    }
    
    sf::FloatRect LightingArea::getGlobalBounds () const{
        return Transformable::getTransform().transformRect(m_areaQuad.getBounds());
    }
    
    void  LightingArea::draw(sf::RenderTarget& t, sf::RenderStates s) const{
        if(m_opacity > 0.f){
            if(m_mode == AMBIENT){
                s.blendMode = sf::BlendAdd;
            }
            s.transform *= Transformable::getTransform();
            s.texture = &m_renderTexture.getTexture();
            t.draw(m_areaQuad, s);
        }
    }
    
    void LightingArea::clear(){
        if(m_baseTexture != nullptr){
            m_renderTexture.clear(sf::Color::Transparent);
            m_renderTexture.draw(m_baseTextureQuad, m_baseTexture);
        }else{
            m_renderTexture.clear(getActualColor());
        }
    }
    
    void LightingArea::setAreaColor(sf::Color c){
        m_color = c;
        sfu::setColor(m_baseTextureQuad, getActualColor());
    }
    
    sf::Color LightingArea::getAreaColor() const{
        return m_color;
    }
    
    sf::Color LightingArea::getActualColor() const{
        sf::Color ret(m_color);
        ret.a *= m_opacity;
        return ret;
    }
    
    void LightingArea::setAreaOpacity(float o){
        m_opacity = o;
        sfu::setColor(m_baseTextureQuad, getActualColor());
    }
    
    float LightingArea::getAreaOpacity() const{
    return m_opacity;
    }
    
    void LightingArea::draw(const LightSource& light){
        if(m_opacity > 0.f && m_mode == FOG){
            sf::RenderStates fogrs;
            fogrs.blendMode = l_substractAlpha;
            fogrs.transform *= Transformable::getTransform().getInverse();
            m_renderTexture.draw(light, fogrs);
        }
    }
    
    void LightingArea::setAreaTexture(const sf::Texture* texture, sf::IntRect rect){
        m_baseTexture = texture;
        if(rect.size.x == 0 && rect.size.y == 0 && texture != nullptr){
            rect.size = sf::Vector2i(texture->getSize());
        }
        initializeRenderTexture(sf::Vector2f(rect.size));
        setTextureRect(rect);
    }
    
    const sf::Texture* LightingArea::getAreaTexture() const{
        return m_baseTexture;
    }
    
    void LightingArea::setTextureRect(const sf::IntRect& rect){
        m_baseTextureQuad[0].texCoords = sf::Vector2f(rect.position);
        m_baseTextureQuad[1].texCoords = sf::Vector2f(rect.position.x, rect.position.y + rect.size.y);
        m_baseTextureQuad[2].texCoords = sf::Vector2f(rect.position.x + rect.size.x, rect.position.y);
        m_baseTextureQuad[3].texCoords = sf::Vector2f(rect.position.x + rect.size.x, rect.position.y + rect.size.y);
    }
    
    sf::IntRect LightingArea::getTextureRect() const{
        return m_baseTextureRect;
    }
    
    void LightingArea::setMode(Mode mode){
        m_mode = mode;
    }
    
    LightingArea::Mode LightingArea::getMode() const{
        return m_mode;
    }
    
    void LightingArea::display(){
        m_renderTexture.display();
    }
}
