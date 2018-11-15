#include "Project_Chess.h"
#include "CGContext/CGMath.h"
#include "Utilities/CG1Helper/CG1Helper.h"
#include "Utilities/CGImageFile.h"

#include <iostream> // only for std::cin/cout
#include <string>
#include <cctype>

Project_Chess::Project_Chess(int w, int h)
	:Application (w,h),frame_res(w,h)
{
	// ...

	// TODO: uncomment this for testing purposes
	//chess.resetToTestCase();
}

// helper function, only for text console demo application:
// print a human-readable repesentation of a CGChessColor value
const char *Project_Chess::color_string(const CGChessColor c)
{
	return (c == CG_CHESS_WHITE)?"white":"black";
}

// helper function, only for text console demo application:
// print a human-readable repesentation of a CGChessMove
const std::string Project_Chess::move_string(const CGChessMove& move)
{

	std::string output="XX-XX";
	output[0] = 'A' + move.from.x;
	output[1] = '1' + move.from.y;
	output[3] = 'A' + move.to.x;
	output[4] = '1' + move.to.y;
	return output;
}

// helper function, only for text console demo application:
// get a chess field from keyboard
// returns true if user gave a valid move, false if abort was requested
bool Project_Chess::get_field_from_keyboard(const std::string& prompt, CGVec2i& field)
{
	bool valid=false;
	int row,column;

	// invalid field
	field.x=-1;
	field.y=-1;

	do {
		std::cout << prompt << " ('q' to abort): " << std::flush;
		std::string user_input;
		std::cin >> user_input;

		// abort with Q or X
		if (user_input.length() >= 1) {
			if (std::tolower(user_input[0]) == 'q' || std::tolower(user_input[0]) == 'x') {
				return false;
			}
		}

		if (user_input.length() == 2) {
			column = std::tolower(user_input[0]) - 'a';
			row = user_input[1] - '1';
			if (column >= 0 && column <= 7 && row >= 0 && row <= 7) {
				valid=true;
			}
		}
	} while(!valid);

	field.x=column;
	field.y=row;
	return true;
}

// helper function, only for text console demo application:
// get a chess move from keyboard
// returns true if user gave a valid move, false if abort was requested
bool Project_Chess::get_move_from_keyboard(const std::string& prompt, CGChessMove& move)
{
	return get_field_from_keyboard(prompt + " from", move.from) && get_field_from_keyboard(prompt + " to", move.to);
}

// the actual program step which is executed in a loop
void Project_Chess::program_step()
{
	// This is only a demonstration for how to use
	// the various CGChess classes and methods!

	bool gameIsFinished = false;

	// visualize the current board
	std::cout << std::endl;
	chess.board().print();

	std::cout << "*** " << color_string(chess.toMove()) << "'s turn ***" << std::endl;

	// inform about "check" condition
	std::vector<CGChessMove> finishing_moves = chess.board().getMovesThreateningKing(chess.toMove());
	if (finishing_moves.size() > 0) {
		std::cout << "CHECK!" <<std::endl;
		for (size_t i=0; i<finishing_moves.size(); i++) {
			std::cout << "    threatening opponent move: " << move_string(finishing_moves[i]) << std::endl;
		}
	}


	// check for finishing conditions
	if (chess.board().is_draw()) {
		std::cout << "Draw!" << std::endl;
		gameIsFinished = true;
	}
	if (chess.board().is_checkmate(chess.toMove())) {
		std::cout << color_string(chess.toMove()) << " is CHECK MATE!" << std::endl;
		gameIsFinished = true;
	}

	if (!gameIsFinished) {
		// get the next move
		CGChessMove move;
		if (chess.toMove() == CG_CHESS_WHITE) {
			// white player is controlled by the user
			bool validMove=false;
			do {
				if (!get_move_from_keyboard("your move", move)) {
					std::cout << "exit requested!" << std::endl;
					CG1Helper::exitAppilication();
					return;
				}
				validMove = chess.testMove(move);
				if (!validMove) {
					std::cout << "invalid move!" << std::endl;
				}
			} while (!validMove);
		} else {
			// black player is simulated by the computer
			move=chess.getAIMove();
			if (!chess.testMove(move)) {
				std::cout << "I give up" << std::endl;
				return;
			}
		}

		// apply the move
		std::cout << color_string(chess.toMove()) << "'s move: " << move_string(move)<< std::endl;
		chess.makeMove(move);
	}

	if (gameIsFinished) {
		// TODO: don't do this in your project,
		// if the game is finished, you should visualize it somehow
		// but the program should go on
		CG1Helper::exitAppilication();
	}
}

