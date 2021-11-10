#pragma once

namespace ar
{
	/// <summary>
	/// This class provides an abstract interface for games.
	/// </summary>
	class Game
	{
	public:
		// Destructor
		virtual ~Game() = default;

		// Functions
		virtual void run()
		{
			while (is_running())
			{
				fixed_update();
				update();
				late_update();
				render();
				if (is_paused())
				{
					on_game_pause();
				}
			}
		}

	protected:
		// Game loop
		virtual void fixed_update() = 0;
		virtual void update() = 0;
		virtual void late_update() = 0;
		virtual void render() = 0;
		virtual void on_game_pause() = 0;

		// State
		virtual bool is_running() const = 0;
		virtual bool is_paused() const = 0;
	};
}
