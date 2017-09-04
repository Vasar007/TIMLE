#ifndef LOADINGSTATE_HPP
#define LOADINGSTATE_HPP

#include "State.hpp"
#include "ParallelTask.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>


class LoadingState : public State
{
	private:
		sf::Text			mLoadingText;
		sf::RectangleShape	mProgressBarBackground;
		sf::RectangleShape	mProgressBar;
	
		ParallelTask		mLoadingTask;


	public:
							LoadingState(StateStack& stack, Context context);
	
		virtual void		draw();
		virtual bool		update(sf::Time dt);
		virtual bool		handleEvent(const sf::Event& event);
	
		void				setCompletion(float percent);
};

#endif // LOADINGSTATE_HPP
