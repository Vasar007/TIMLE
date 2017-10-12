#ifndef LOADINGSTATE_HPP
#define LOADINGSTATE_HPP

#include "State.hpp"
#include "ParallelTask.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>


class LoadingState final : public State
{
	private:
		sf::Text			mLoadingText;
		sf::RectangleShape	mProgressBarBackground;
		sf::RectangleShape	mProgressBar;
	
		ParallelTask		mLoadingTask;


	public:
							LoadingState(StateStack& stack, Context context);
	
		void				draw() override;
		bool				update(sf::Time dt) override;
		bool				handleEvent(const sf::Event& event) override;
	
		void				setCompletion(float percent);
};

#endif // LOADINGSTATE_HPP
