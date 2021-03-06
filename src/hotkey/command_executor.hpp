/*
   Copyright (C) 2003 - 2016 by David White <dave@whitevine.net>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef COMMAND_EXECUTOR_HPP_INCLUDED
#define COMMAND_EXECUTOR_HPP_INCLUDED

#include "hotkey_command.hpp"
#include "game_end_exceptions.hpp"
#include "events.hpp"

class display;
class CVideo;

namespace hotkey {

enum ACTION_STATE { ACTION_STATELESS, ACTION_ON, ACTION_OFF, ACTION_SELECTED, ACTION_DESELECTED };

// Abstract base class for objects that implement the ability
// to execute hotkey commands.
class command_executor
{

protected:
	virtual ~command_executor() {}

public:
	virtual void cycle_units() {}
	virtual void cycle_back_units() {}
	virtual void end_turn() {}
	virtual void goto_leader() {}
	virtual void unit_hold_position() {}
	virtual void end_unit_turn() {}
	virtual void undo() {}
	virtual void redo() {}
	virtual void terrain_description() {}
	virtual void unit_description() {}
	virtual void rename_unit() {}
	virtual void save_game() {}
	virtual void save_replay() {}
	virtual void save_map() {}
	virtual void load_game() {}
	virtual void toggle_ellipses() {}
	virtual void toggle_grid() {}
	virtual void status_table() {}
	virtual void recall() {}
	virtual void recruit() {}
	virtual void repeat_recruit() {}
	virtual void speak() {}
	virtual void whisper() {}
	virtual void shout() {}
	virtual void create_unit() {}
	virtual void change_side() {}
	virtual void kill_unit() {}
	virtual void preferences() {}
	virtual void objectives() {}
	virtual void unit_list() {}
	virtual void show_statistics() {}
	virtual void stop_network() {}
	virtual void start_network() {}
	virtual void label_terrain(bool /*team_only*/) {}
	virtual void clear_labels() {}
	virtual void label_settings() {}
	virtual void show_enemy_moves(bool /*ignore_units*/) {}
	virtual void toggle_shroud_updates() {}
	virtual void update_shroud_now() {}
	virtual void continue_move() {}
	virtual void search() {}
	virtual void show_help() {}
	virtual void show_chat_log() {}
	virtual void user_command() {}
	virtual void custom_command() {}
	virtual void ai_formula() {}
	virtual void clear_messages() {}
	virtual void change_language() {}
	virtual void play_replay() {  }
	virtual void reset_replay() {}
	virtual void stop_replay() {}
	virtual void replay_next_turn() {  }
	virtual void replay_next_side() {  }
	virtual void replay_next_move() {  }
	virtual void replay_show_everything() {}
	virtual void replay_show_each() {}
	virtual void replay_show_team1() {}
	virtual void replay_skip_animation() {}
	virtual void replay_exit() {}
	virtual void whiteboard_toggle() {}
	virtual void whiteboard_execute_action() {}
	virtual void whiteboard_execute_all_actions() {}
	virtual void whiteboard_delete_action() {}
	virtual void whiteboard_bump_up_action() {}
	virtual void whiteboard_bump_down_action() {}
	virtual void whiteboard_suppose_dead() {}
	virtual void select_hex() {}
	virtual void deselect_hex() {}
	virtual void move_action() {}
	virtual void select_and_action() {}
	virtual void left_mouse_click() {}
	virtual void right_mouse_click() {}
	virtual void toggle_accelerated_speed() {}
	virtual void lua_console();
	virtual void zoom_in() {}
	virtual void zoom_out() {}
	virtual void zoom_default() {}
	virtual void map_screenshot() {}

	virtual void set_button_state() {}
	virtual void recalculate_minimap() {}
	virtual CVideo& get_video() = 0;

	// execute_command's parameter is changed to "hotkey_command& command" and this not maybe that is too inconsitent.
	// Gets the action's image (if any). Displayed left of the action text in menus.
	virtual std::string get_action_image(hotkey::HOTKEY_COMMAND /*command*/, int /*index*/) const { return ""; }
	// Does the action control a toggle switch? If so, return the state of the action (on or off).
	virtual ACTION_STATE get_action_state(hotkey::HOTKEY_COMMAND /*command*/, int /*index*/) const { return ACTION_STATELESS; }
	// Returns the appropriate menu image. Checkable items will get a checked/unchecked image.
	std::string get_menu_image(display& disp, const std::string& command, int index=-1) const;
	// Returns a vector of images for a given menu.
	std::vector<std::string> get_menu_images(display &, const std::vector<std::string>& items_arg);

	virtual void show_menu(const std::vector<std::string>& items_arg, int xloc, int yloc, bool context_menu, display& gui);
	void execute_action(const std::vector<std::string>& items_arg, int xloc, int yloc, bool context_menu, display& gui);

	virtual bool can_execute_command(const hotkey_command& command, int index=-1) const = 0;
	virtual bool execute_command(const hotkey_command& command, int index=-1);
};
class command_executor_default : public command_executor
{
protected:
	static const int zoom_amount = 4;
	virtual display& get_display() = 0;
public:
	CVideo& get_video();
	void set_button_state();
	void recalculate_minimap();
	void lua_console();
	void zoom_in();
	void zoom_out();
	void zoom_default();
	void map_screenshot();
	void quit_to_main_menu();
};
/* Functions to be called every time a event is intercepted.
 * Will call the relevant function in executor if the event is not NULL.
 * Also handles some events in the function itself,
 * and so is still meaningful to call with executor=NULL
 */
void jbutton_event(const SDL_Event& event, command_executor* executor);
void jhat_event(const SDL_Event& event, command_executor* executor);
void key_event(const SDL_Event& event, command_executor* executor);
void mbutton_event(const SDL_Event& event, command_executor* executor);


//TODO
void execute_command(const hotkey_command& command, command_executor* executor, int index=-1);

// Object which will ensure that basic keyboard events like escape
// are handled properly for the duration of its lifetime.
class basic_handler : public events::sdl_handler
{
public:
	basic_handler(command_executor* exec);

	void handle_event(const SDL_Event& event);
#if SDL_VERSION_ATLEAST(2, 0, 0)
	void handle_window_event(const SDL_Event&) {}
#endif

private:
	command_executor* exec_;
};


}
#endif
