#ifndef GALAGA_MENU_OPTION_H
#define GALAGA_MENU_OPTION_H

#include <string>
#include <memory>
#include <functional>

#include <Core/ECS/Component.h>
#include <Texture2D.h>

namespace Engine
{
    class TextComponent;
    class TextureRenderer;
    class GameObject;
}

namespace Galaga
{
    using Engine::GameObject;
    using Engine::TextComponent;
    using Engine::TextureRenderer;
    using Engine::Texture2D;

    class MenuOption final : public Engine::Component
    {
    public:
        MenuOption(GameObject* object);
        MenuOption(GameObject* object, const std::string& text, const std::function<void()>& onClick, bool isSelected = false);
		virtual ~MenuOption() = default;

		MenuOption(const MenuOption& other) = delete;
		MenuOption(MenuOption&& other) = delete;
		MenuOption& operator=(const MenuOption& other) = delete;
		MenuOption& operator=(MenuOption&& other) = delete;

		virtual void Init() override;
		virtual void Update() override {};
		virtual void Render() override {};

        void Click();
        void Select();
        void Deselect();

    private:
        std::function<void()> m_OnClick{[](){}};

        Engine::TextComponent* m_TextComponent{};
        Engine::TextureRenderer* m_TextureRenderer{};
        std::shared_ptr<Engine::Texture2D> m_SelectedTexture{};

        std::string m_Text{};
        bool m_IsSelected{false};
    };
}

#endif //GALAGA_MENU_OPTION_H