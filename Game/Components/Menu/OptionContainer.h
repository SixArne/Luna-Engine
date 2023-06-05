#ifndef GALAGA_OPTION_CONTAINER
#define GALAGA_OPTION_CONTAINER

#include <Core/ECS/Component.h>
#include "MenuOption.h"

namespace Galaga
{
    class OptionContainer final : public Engine::Component
    {
    public:
        using Options = std::vector<MenuOption*>;

        OptionContainer(GameObject* object);
        OptionContainer(GameObject* object, Options& options);
		virtual ~OptionContainer() = default;

		OptionContainer(const OptionContainer& other) = delete;
		OptionContainer(OptionContainer&& other) = delete;
		OptionContainer& operator=(const OptionContainer& other) = delete;
		OptionContainer& operator=(OptionContainer&& other) = delete;

        virtual void Init() override {};
        virtual void Update() override {};
        virtual void Render() override {};

        void Next();
        void Previous();
        void Click();

        void AddOption(MenuOption* option);

    private:
        Options m_Options{};

        int m_CurrentIndex{};
    };
}

#endif //GALAGA_OPTION_CONTAINER