#include <iostream>

class Engine
{
    public:

    enum GameState{ GS_State1, GS_State2};

    using predicate_t = bool (Engine::*)(GameState, GameState) const;

    public:
    
    void ToggleState
        (Engine* pEngine, predicate_t Pred,
             GameState state_1, GameState state_2)
    {
        std::cout << "Object is pointer" << std::endl;

        bool state = (pEngine->*Pred)(state_1, state_2);
        if(state == true)
        {
            // pause
        }
        else
        {
            // resume
        }
    }

    void ToggleState
       (Engine& engine,  predicate_t Pred,
             GameState state_1, GameState state_2)
    {
        std::cout << "Object is reference" << std::endl;
        
        auto state = (engine.*Pred)(state_1, state_2);
        if(state == true)
        {
            // pause
        }
        else
        {
            // resume
        }
    }

    //bool (Engine::*)(GameState, GameState) const;
    bool predicate_1(GameState state1, GameState state2) const
    {
        return state1 == state2;
    }

    bool predicate_2(GameState state1, GameState state2) const
    {
        return state1 != state2;
    }
};

int main()
{
    Engine e, a, b;

    e.ToggleState(e, &Engine::resume,
        Engine::GameState::Pause, Engine::GameState::Resume);

     e.ToggleState(&a, &Engine::pause,
        Engine::GameState::Pause, Engine::GameState::Resume);

}