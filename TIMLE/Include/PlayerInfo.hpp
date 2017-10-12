#ifndef PLAYERINFO_HPP
#define PLAYERINFO_HPP

#include "DataTables.hpp"
#include "Foreach.hpp"

#include <SFML/Window/Event.hpp>

#include <map>


/**
 * \brief Class, which contains and controls all player's data.
 */
class PlayerInfo final
{
	public:
		/**
		 * \brief Array of action's player states.
		 */
		enum Action
		{
			MoveLeft,
			MoveRight,
			MoveUp,
			MoveDown,
			Fire,
			LaunchMissile,
			ActionCount
		};

		/**
		 * \brief Array of the game status, according to the player's state.
		 */
		enum GameStatus
		{
			GameRunning,
			LevelComplete,
			GameOver
		};


		/**
		 * \brief Data structure that keeping player's save point.
		 */
		struct SavePoint
		{
			SavePoint(float x, float y)
				: x(x)
				, y(y)
			{
			}

			float x;
			float y;
		};

	
	private:
		/**
		 * \brief Associative array of the key buttons for each player's action.
		 */
		std::map<sf::Keyboard::Key, Action>	mKeyBinding;

		//std::map<Action, Command>			mActionBinding;	// Not using now.

		/**
		 * \brief Current game status, using for interactiving with world.
		 */
		GameStatus 							mCurrentGameStatus;

		/**
		 * \brief Current level number.
		 */
		size_t								mCurrentLevelNumber;

		/**
		 * \brief Current player's class for interacting.
		 */
		Player*								mPlayer;


	public:
		/**
		 * \brief Current dialog number, which we need to show.
		 */
		size_t								mDialogNumber;

		/**
		 * \brief Array of all showed dialogs, which we have shown yet.
		 */
		std::vector<size_t>					mShowedDialogs;

		/**
		 * \brief Variable for choosing number in the choosing state of dialog.
		 */
		size_t								mChoosingNumber;

		/**
		 * \brief All choices, which player has done yet.
		 */
		std::vector<size_t>					mChosenSolution;

		/**
		 * \brief Array of the quests, which player can carry out.
		 */
		std::vector<bool>					mQuests;

		/**
		 * \brief Boolean flag for notification of completion downloading level and resources.
		 */
		bool								mLoaded;

		/**
		 * \brief Lives counter of the player's hero.
		 */
		size_t								mLivesCount;

		/**
		 * \brief Last save point of the current game.
		 */
		SavePoint							mLastSavePoint;

		/**
		 * \brief Define we can ressurect player's hero or not.
		 */
		bool								mCanRessurect;


	private:
		/**
		 * \brief Default starter binding keys with action (action is only function<void>).
		 */
		void				initializeActions();

		/**
		 * \brief			Check if sending action is realtime action or not.
		 * \param action	Action for checking.
		 * \return			True if action is realtime or false otherwise.
		 */
		static bool			isRealtimeAction(Action action);


	public:
		/**
		 * \brief Default constructor.
		 */
							PlayerInfo();
	
		/**
		 * \brief			Sends a notification to dialog state to show a certain dialog.
		 * \param number	Number of the necessary dialog.
		 */
		void				showDialog(size_t number);

		/**
		 * \brief			Bind action with key.
		 * \param action	Action for binding.
		 * \param key		Key for binding.
		 */
		void				assignKey(Action action, sf::Keyboard::Key key);

		/**
		 * \brief			Get key that assigned with current action.
		 * \param action	Action for searching assigned key.
		 * \return			Found key or Key::Unknown.
		 */
		sf::Keyboard::Key	getAssignedKey(Action action) const;
	
		/**
		 * \brief			Change current game status of the app.
		 * \param status	New game status.
		 */
		void 				setGameStatus(GameStatus status);


		/**
		 * \brief	Get current game status (Needs for different checking).
		 * \return	Current game status.
		 */
		GameStatus 			getGameStatus() const;

		/**
		 * \brief			Change current level number.
		 * \param number	New level number.
		 */
		void 				setLevelNumber(size_t number);

		/**
		 * \brief	Get current level number.
		 * \return	Current level number.
		 */
		size_t 				getLevelNumber() const;

		/**
		 * \brief			Change current player's class.
		 * \param player	Pointer to a new player's class for interacting.
		 */
		void				setPlayer(Player* player);

		/**
		 * \brief	Get current binded player's class.
		 * \return	Pointer to current player's class.
		 */
		Player*				getPlayer() const;

		/**
		 * \brief Reset all data about current game session.
		 */
		void				resetData();

		/**
		 * \brief Reset some player's data (not reset ALL data) and ressurect at last save point.
		 */
		void				ressurectPlayer();
};

#endif // PLAYERINFO_HPP
