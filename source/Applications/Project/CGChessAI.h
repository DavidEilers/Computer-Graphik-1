#pragma once

#include "CGChessSet.h"
#include <limits>
#include <cstdlib>
class CGChessAI
{
public:
	/**
	 * max_depth is the maximum depth the alpha-beta search will be performed
	 * to. Higher makes for a smarter AI but is mutch slower.
	 */
	CGChessAI(const unsigned int max_depth = 3)
	{
		maxDepth = max_depth;
	}

	/**
	 * makeAMove makes a move on the board for the specified color.
	 * Returns the move that was made. When the AI thinks it is a draw a move
	 * mit from<0 and to<0 is returned. If the AI thinks it will loose it moves
	 * its king to (-1,-1).
	 */
	CGChessMove getAMove(const CGChessBoard& board, const CGChessColor color);
protected:

	unsigned int maxDepth;
	CGChessMove nextMove;

	float Max(const CGChessBoard &board, unsigned int depth, float alpha, float beta, int currentValue);
	float Min(const CGChessBoard &board, unsigned int depth, float alpha, float beta, int currentValue);

	int evaluateBoard(const CGChessBoard &board);
	float evaluateBoard(const CGChessBoard &board, int currentValue, int depth);

};
