#pragma once
#include "CGContext/CGVector.h"
#include <cstdio>
#include <assert.h>
#include <vector>

class CGChessMove
{
public:
	CGVec2i from;
	CGVec2i to;

	CGChessMove(const CGVec2i& from=CGVec2i(0,0),
		    const CGVec2i& to=CGVec2i(0,0))
		:from(from),to(to){}
	bool operator==(const CGChessMove& o)const
	{
		return  o.from == from && o.to == to;
	}
};

enum CGChessPieceID
{
	CG_CHESS_KING  ,
	CG_CHESS_QUEEN ,
	CG_CHESS_ROOK  ,
	CG_CHESS_BISHOP,
	CG_CHESS_KNIGHT,
	CG_CHESS_PAWN  ,
	CG_CHESS_EMPTY ,

};



enum CGChessColor
{
	CG_CHESS_BLACK=0,
	CG_CHESS_WHITE
};


class CGChessPiece
{


public:
	CGChessColor color;
	CGChessPieceID piece;
	int value;
	CGChessPiece(const CGChessColor color = CG_CHESS_BLACK,
		     const CGChessPieceID id = CG_CHESS_EMPTY)
		:color(color)
	{
		setPiece(id);
	}

	bool empty() const {return piece == CG_CHESS_EMPTY;}
	void print() const
	{
		if(color == CG_CHESS_BLACK)
		{
			switch (piece)
			{
			case CG_CHESS_KING  : printf("Kb"); return;
			case CG_CHESS_QUEEN : printf("Qb"); return;
			case CG_CHESS_ROOK  : printf("Rb"); return;
			case CG_CHESS_BISHOP: printf("Bb"); return;
			case CG_CHESS_KNIGHT: printf("kb"); return;
			case CG_CHESS_PAWN  : printf("Pb"); return;
			case CG_CHESS_EMPTY : printf("  "); return;
			}
		}
		else
		{
			switch (piece)
			{
			case CG_CHESS_KING  : printf("Kw"); return;
			case CG_CHESS_QUEEN : printf("Qw"); return;
			case CG_CHESS_ROOK  : printf("Rw"); return;
			case CG_CHESS_BISHOP: printf("Bw"); return;
			case CG_CHESS_KNIGHT: printf("kw"); return;
			case CG_CHESS_PAWN  : printf("Pw"); return;
			case CG_CHESS_EMPTY : printf("  "); return;
			}
		}
	}
	void setPiece(const CGChessPieceID id)
	{
		int valueModifier = (color == CG_CHESS_BLACK)?-1:1;
		piece=id;
		switch(id)
		{
		case CG_CHESS_KING  : value=valueModifier * 20; break;
		case CG_CHESS_QUEEN : value=valueModifier * 15; break;
		case CG_CHESS_ROOK  : value=valueModifier * 3; break;
		case CG_CHESS_BISHOP: value=valueModifier * 3; break;
		case CG_CHESS_KNIGHT: value=valueModifier * 3; break;
		case CG_CHESS_PAWN  : value=valueModifier * 1; break;
		case CG_CHESS_EMPTY : value=0;
		}
	}

};

class CGChessBoard
{
public:


	CGChessBoard();
	CGChessBoard(const CGChessBoard& o);

	/**
	 * @brief resets the board for a new game.
	 */
	void reset();

	/**
	 * @brief resets the board to a test scenario
	 */
	void resetToTestCase();

	/**
	 * @brief makeMove makes a move. There are no checks whether the move is
	 * valid or not.
	 * @param mv The move to make
	 * @return The piece that stood at the mv.to location. An empty piece if the
	 * move ended on an empty field.
	 */
	CGChessPiece makeMove(const CGChessMove& mv);


	/**
	 * @brief at Gives access to the field on the board.
	 */
	const CGChessPiece& at(const CGVec2i& p)const {return at(p[0],p[1]);}
	const CGChessPiece& at(int x, int y)const {assert(x>=0 && x <8 && y>=0 && y < 8);return values[x][y];}

	CGChessPiece& at(const CGVec2i& p){return at(p[0],p[1]);}
	CGChessPiece& at(int x, int y) {assert(x>=0 && x <8 && y>=0 && y < 8);return values[x][y];}

	/**
	 * @brief find gives the location of a given piece
	 * @param id CGChessPieceID of the piece (the type)
	 * @param clr The color of the piece
	 * @return The location of the first appearance of the specified piece or
	 * (-1,-1) if it was not found.
	 */
	CGVec2i find(CGChessPieceID id, CGChessColor clr)const;


	/**
	 * @brief is_draw test if the board is in some kind of draw state.
	 * Note: This analysis is not complete it is a rough guess, but it is
	 * close enough for this example.
	 * @return
	 */
	bool is_draw() const;

	/**
	 * @brief is_checkmate tests if the board is in a check mate state
	 * for any player
	 * @return
	 */
	bool is_checkmate() const;

	/**
	 * @brief is_checkmate tests if the board is in a check mate state
	 * for the player color, meaning, that this player has lost
	 * @return
	 */
	bool is_checkmate(const CGChessColor color) const;

	/**
	 * @brief getPossibleMoves gives all possible moves a figure from a given
	 * position on the board can perform
	 * @param at
	 * @return
	 */
	std::vector<CGChessMove> getPossibleMoves(const CGVec2i& at) const;

	/**
	 * @brief generatePossibleMoves generates all moves a player of a specific
	 * color can perform. (Those include moves, that bring the own King into check)
	 * @param color
	 * @param ret_val
	 */
	void generatePossibleMoves(const CGChessColor color, std::vector<CGChessMove>& ret_val)const;

	/**
	 * @brief removeCheckMoves filters all moves, that bring the own king into check
	 * from the given vector of moves.
	 * @param moves
	 */
	void removeCheckMoves(std::vector<CGChessMove> &moves) const;

	/**
	 * @brief print the board to the text console
	 */
	void print() const;

	/**
	 * @brief getMovesThreateningKing returns all moves the opponent
	 * could perfom to beat the king. If the returned vector is empty the
	 * player is not in "check" state.
	 * @param color of the King in question
	 * @return vector of all the moves beating the king.
	 */
	std::vector<CGChessMove> getMovesThreateningKing(const CGChessColor color) const;

	/**
	 * @brief is_valid_move returns true, if the move is possible on this board,
	 * testing if the moves origin is on the board and if it is allowed by
	 * the rules (concidering the chess piece's capabilities and not moving
	 * your own king in check). If the move starts on an empty field the
	 * move is concidered invalid!
	 * @param move
	 * @return
	 */
	bool is_valid_move(const CGChessMove& move) const;

	/**
	 * @brief is_possible_move test if the move could be performed by the
	 * selected chess piece.
	 * @param move
	 * @return false if there is no piece at move.from or if the piece is
	 * not allowed to perform this move.
	 */
	bool is_possible_move(const CGChessMove& move) const;
	/**
	 * @brief will_move_in_check test if the move will bring the own king
	 * into check.
	 * @param move
	 * @return
	 */
	bool will_move_in_check(const CGChessMove& move) const;
protected:

	CGChessPiece values[8][8];
	unsigned int no_real_move;

	void generatePossibleMovesKing(const CGChessColor color, const CGVec2i& at, std::vector<CGChessMove>& ret_val) const;
	void generatePossibleMovesQueen(const CGChessColor color, const CGVec2i& at, std::vector<CGChessMove>& ret_val)const;
	void generatePossibleMovesRook(const CGChessColor color, const CGVec2i& at, std::vector<CGChessMove>& ret_val)const;
	void generatePossibleMovesBishop(const CGChessColor color, const CGVec2i& at, std::vector<CGChessMove>& ret_val)const;
	void generatePossibleMovesKnight(const CGChessColor color, const CGVec2i& at, std::vector<CGChessMove>& ret_val)const;
	void generatePossibleMovesPawn(const CGChessColor color, const CGVec2i& at, std::vector<CGChessMove>& ret_val)const;


	void generatePossibleMoves(const CGChessColor color, const CGVec2i& at, std::vector<CGChessMove>& ret_val)const;
};
