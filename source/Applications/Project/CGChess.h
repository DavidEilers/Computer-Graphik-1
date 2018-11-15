#pragma once
#include <vector>
#include "CGContext/CGVector.h"
#include "CGContext/CG.h"
#include <random>
#include <assert.h>

#include "CGChessSet.h"
#include "CGChessAI.h"
class CGChessSystem
{

protected:
	CGChessBoard m_board;
	CGChessAI	 m_ai;
	CGChessColor m_to_move;
public:

	CGChessSystem()
	{
		m_to_move = CG_CHESS_WHITE;
	}

	void restartGame()
	{
		m_board.reset();
		m_to_move = CG_CHESS_WHITE;
	}

	/// Gives acces to the current chess board.
	const CGChessBoard& board()const;


	/// Returns the color of the player whos turn is it to
	CGChessColor toMove() const;


	/// Makes a move. Returns true if the move was possible
	/// or false if it wasn't possible.
	bool makeMove(const CGChessMove& move);


	/// Simulates a move, but leaves the board unchanged!
	/// Returns true if the move is possible or false if it is not.
	bool testMove(const CGChessMove& move) const;


	/// Asks the AI for its move. For the current player.
	CGChessMove getAIMove();

	/// Resets the game to a pre-defined test case
	void resetToTestCase();

	/// The geometry for the different Pieces
	const float* getNormals(const CGChessPieceID cp) const ;
	const float* getPositions(const CGChessPieceID cp)const ;
	const float* getTexCoords(const CGChessPieceID cp) const ;
	unsigned int getNumVerts(const CGChessPieceID cp)const ;
	CGPrimitiveType getPrimitve(const CGChessPieceID cp) const;
};
