#pragma once
#include "Applications/Application.h"
#include "CGContext/CGProgram.h"
#include "CGChess.h"


class Project_Chess :public Application {
	CGChessSystem chess;
	CGVec2i frame_res;

	// helper functions, only for text console demo application
	const char *color_string(const CGChessColor c);
	const std::string move_string(const CGChessMove& move);
	bool get_field_from_keyboard(const std::string& prompt, CGVec2i& field);
	bool get_move_from_keyboard(const std::string& prompt, CGChessMove& move);

public:
	Project_Chess(int w, int h);
	void program_step();
};

