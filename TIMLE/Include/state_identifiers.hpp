#ifndef STATE_IDENTIFIERS_HPP
#define STATE_IDENTIFIERS_HPP


namespace States
{
    enum class ID
    {
        None,
        Title,
        Menu,
        Game,
        Loading,
        Pause,
        Settings,
        GameOver,
        Dialog,
        Choosing,
        Titre,
        ChangingSettings,
        Transit
    };
}

#endif // STATE_IDENTIFIERS_HPP
