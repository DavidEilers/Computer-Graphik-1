#include "CGChessSet.h"

//------------------------------------------------------------------------------
bool outCB(const CGVec2i& p){return p[0]<0 || p[1] <0 || p[0] >7 || p[1] > 7;}
//------------------------------------------------------------------------------
CGChessBoard::CGChessBoard()
{
	reset();
}
//------------------------------------------------------------------------------
CGChessBoard::CGChessBoard(const CGChessBoard &o)
{
	for(unsigned int i = 0; i < 8;i++)
		for(unsigned int j = 0; j < 8;j++)
			values[i][j] = o.values[i][j];
}
//------------------------------------------------------------------------------
void CGChessBoard::reset()
{
	for(unsigned int i = 0; i < 8;i++) {
		for(unsigned int j = 0; j < 8;j++) {
			values[i][j].piece = CG_CHESS_EMPTY;
			values[i][j].value = 0;
		}
	}

	no_real_move = 0;
	for(int i =0 ; i< 8;i++)
	{
		for (int j=0; j<2; j++)
		{
			values[i][j].color = CG_CHESS_WHITE;
			values[i][7-j].color = CG_CHESS_BLACK;
		}
		values[i][1].setPiece(CG_CHESS_PAWN);
		values[i][6].setPiece(CG_CHESS_PAWN);
	}

	values[0][0].setPiece(CG_CHESS_ROOK);
	values[1][0].setPiece(CG_CHESS_KNIGHT);
	values[2][0].setPiece(CG_CHESS_BISHOP);
	values[3][0].setPiece(CG_CHESS_QUEEN);
	values[4][0].setPiece(CG_CHESS_KING);
	values[5][0].setPiece(CG_CHESS_BISHOP);
	values[6][0].setPiece(CG_CHESS_KNIGHT);
	values[7][0].setPiece(CG_CHESS_ROOK);

	values[0][7].setPiece(CG_CHESS_ROOK);
	values[1][7].setPiece(CG_CHESS_KNIGHT);
	values[2][7].setPiece(CG_CHESS_BISHOP);
	values[3][7].setPiece(CG_CHESS_QUEEN);
	values[4][7].setPiece(CG_CHESS_KING);
	values[5][7].setPiece(CG_CHESS_BISHOP);
	values[6][7].setPiece(CG_CHESS_KNIGHT);
	values[7][7].setPiece(CG_CHESS_ROOK);
}
//------------------------------------------------------------------------------
void CGChessBoard::resetToTestCase()
{
	for(unsigned int i = 0; i < 8;i++) {
		for(unsigned int j = 0; j < 8;j++) {
			values[i][j].piece = CG_CHESS_EMPTY;
			values[i][j].value = 0;
		}
	}

	no_real_move = 0;

	// white pieces
	values[0][7].setPiece(CG_CHESS_ROOK);
	values[0][7].color = CG_CHESS_WHITE;
	values[0][5].setPiece(CG_CHESS_ROOK);
	values[0][5].color = CG_CHESS_WHITE;
	values[2][4].setPiece(CG_CHESS_QUEEN);
	values[2][4].color = CG_CHESS_WHITE;
	values[4][4].setPiece(CG_CHESS_KING);
	values[4][4].color = CG_CHESS_WHITE;

	// black pieces
	values[2][5].setPiece(CG_CHESS_KNIGHT);
	values[2][5].color = CG_CHESS_BLACK;
	values[2][6].setPiece(CG_CHESS_KING);
	values[2][6].color = CG_CHESS_BLACK;
}
//------------------------------------------------------------------------------
CGChessPiece CGChessBoard::makeMove(const CGChessMove &mv)
{
	if(mv.from[0]<0)
		return CGChessPiece(CG_CHESS_BLACK,CG_CHESS_EMPTY);

	if(mv.to[0]<0)
	{
		auto it = at(mv.from);
		values[mv.from[0]][mv.from[1]].setPiece(CG_CHESS_EMPTY);
		return  it;
	}

	auto it = at(mv.to);

	if(at(mv.from).piece == CG_CHESS_PAWN || !it.empty())
		no_real_move=0;
	else
		no_real_move++;

	values[mv.to[0]][mv.to[1]] = values[mv.from[0]][mv.from[1]];
	values[mv.from[0]][mv.from[1]].setPiece(CG_CHESS_EMPTY);

	if(at(mv.to).piece==CG_CHESS_PAWN && (mv.to[1] == 7 || mv.to[1] == 0))
	{
		values[mv.to[0]][mv.to[1]].setPiece(CG_CHESS_QUEEN);
	}
	return it;
}
//------------------------------------------------------------------------------
CGVec2i CGChessBoard::find(CGChessPieceID id, CGChessColor clr) const
{
	for(int i = 0; i < 8;i++)
		for(int j = 0; j < 8;j++)
		{
			const auto& it = at(i,j);
			if(!it.empty() && it.color == clr && it.piece == id)
				return  CGVec2i(i,j);

		}
	return  CGVec2i(-1,-1);
}
//------------------------------------------------------------------------------
bool CGChessBoard::is_draw() const
{
	if(is_checkmate())
		return false;
	if(no_real_move >= 50)
	{
		return true;
	}

	int num_pieces = 0;
	for(int i = 0; i < 8;i++)
		for(int j = 0; j < 8;j++)
		{
			if(!at(i,j).empty())
				num_pieces++;
		}
	if(num_pieces == 2)
	{
		return true;
	}


	std::vector<CGChessMove> a_moves;
	generatePossibleMoves(CG_CHESS_BLACK,a_moves);
	std::vector<CGChessMove> b_moves;
	generatePossibleMoves(CG_CHESS_WHITE,b_moves);
	if(a_moves.empty() && b_moves.empty())
	{
		return true;
	}

	return  false;
}
//------------------------------------------------------------------------------
bool CGChessBoard::is_checkmate() const
{
	return is_checkmate(CG_CHESS_BLACK) || is_checkmate(CG_CHESS_WHITE);
}
//------------------------------------------------------------------------------
bool CGChessBoard::is_checkmate(const CGChessColor color) const
{
	const auto king= find(CG_CHESS_KING,color);
	if(king[0] <0 || king[1] <0)
		return true;
	std::vector<CGChessMove> possibleMoves;
	generatePossibleMoves(color, possibleMoves);
	removeCheckMoves(possibleMoves);
	return possibleMoves.empty();
}
//------------------------------------------------------------------------------
std::vector<CGChessMove> CGChessBoard::getPossibleMoves(const CGVec2i &at) const
{
	std::vector<CGChessMove> ret_val;
	const CGChessColor color = this->at(at).color;
	generatePossibleMoves(color,at,ret_val);
	removeCheckMoves(ret_val);
	return ret_val;
}
//------------------------------------------------------------------------------
void CGChessBoard::generatePossibleMovesKing(const CGChessColor color,
					     const CGVec2i &at,
					     std::vector<CGChessMove> &ret_val) const
{

	for(int x = at[0]-1;x<=at[0]+1;x++)
		for(int y = at[1]-1;y<=at[1]+1;y++)
		{
			CGVec2i curr(x,y);
			if( outCB(curr) || curr == at)
				continue;

			const auto& it = this->at(x,y);
			if(it.empty() || it.color != color)
				ret_val.push_back(CGChessMove(at,curr));
		}
}

//------------------------------------------------------------------------------
void CGChessBoard::generatePossibleMovesQueen(const CGChessColor color,
					      const CGVec2i &at,
					      std::vector<CGChessMove> &ret_val) const
{
	const CGVec2i dirs[]={
		CGVec2i( 1, 0),
		CGVec2i( 0, 1),
		CGVec2i(-1, 0),
		CGVec2i( 0,-1),

		CGVec2i( 1, 1),
		CGVec2i( 1,-1),
		CGVec2i(-1, 1),
		CGVec2i(-1,-1)
       };

	for(int i = 0; i< 8;i++)
	{
		const CGVec2i& dir = dirs[i];
		CGVec2i curr = at+dir;

		while(!outCB(curr))
		{
			const auto& it = this->at(curr);
			if(it.empty())
				ret_val.push_back( CGChessMove(at,curr));
			else
			{
				if(it.color != color)
					ret_val.push_back( CGChessMove(at,curr));
				break;
			}
			curr =curr+dir;
		}
	}

}
//------------------------------------------------------------------------------
void CGChessBoard::generatePossibleMovesRook(const CGChessColor color,
					     const CGVec2i &at,
					     std::vector<CGChessMove> &ret_val) const
{
	const CGVec2i dirs[]={
		CGVec2i( 1, 0),
		CGVec2i( 0, 1),
		CGVec2i(-1, 0),
		CGVec2i( 0,-1),
	};

	for(int i = 0; i< 4;i++)
	{
		const CGVec2i& dir = dirs[i];
		CGVec2i curr = at+dir;

		while(!outCB(curr))
		{
			const auto& it = this->at(curr);
			if(it.empty())
				ret_val.push_back( CGChessMove(at,curr));
			else
			{
				if(it.color != color)
					ret_val.push_back( CGChessMove(at,curr));
				break;
			}
			curr =curr+dir;
		}
	}
}
//------------------------------------------------------------------------------
void CGChessBoard::generatePossibleMovesBishop(const CGChessColor color,
					       const CGVec2i &at,
					       std::vector<CGChessMove> &ret_val) const
{

	const CGVec2i dirs[]={
		CGVec2i( 1, 1),
		CGVec2i( 1,-1),
		CGVec2i(-1, 1),
		CGVec2i(-1,-1)
	};

	for(int i = 0; i< 4;i++)
	{
		const CGVec2i& dir = dirs[i];
		CGVec2i curr = at+dir;

		while(!outCB(curr))
		{
			const auto& it = this->at(curr);
			if(it.empty())
				ret_val.push_back( CGChessMove(at,curr));
			else
			{
				if(it.color != color)
					ret_val.push_back( CGChessMove(at,curr));
				break;
			}
			curr =curr+dir;
		}
	}

}
//------------------------------------------------------------------------------
void CGChessBoard::generatePossibleMovesKnight(const CGChessColor color,
					       const CGVec2i &at,
					       std::vector<CGChessMove> &ret_val) const
{
	const CGVec2i dirs[]={
		CGVec2i( 1, 2),
		CGVec2i( 2, 1),
		CGVec2i( -1, 2),
		CGVec2i( -2, 1),

		CGVec2i( 1, -2),
		CGVec2i( 2, -1),
		CGVec2i( -1, -2),
		CGVec2i( -2, -1),
	};

	for(int i = 0; i< 8;i++)
	{
		const CGVec2i& dir = dirs[i];
		CGVec2i curr = at+dir;

		if(!outCB(curr))
		{
			const auto& it = this->at(curr);
			if(it.empty())
				ret_val.push_back( CGChessMove(at,curr));
			else
			{
				if(it.color != color)
					ret_val.push_back( CGChessMove(at,curr));
			}
		}
	}
}
//------------------------------------------------------------------------------
void CGChessBoard::generatePossibleMovesPawn(const CGChessColor color,
					     const CGVec2i &at,
					     std::vector<CGChessMove> &ret_val) const
{

	const CGVec2i dir = color==CG_CHESS_BLACK?CGVec2i(0,-1):CGVec2i(0,1);
	int steps = 1;

	if(color == CG_CHESS_BLACK && at[1] == 6)
		steps = 2;
	if(color == CG_CHESS_WHITE && at[1] == 1)
		steps = 2;

	CGVec2i curr = at+dir;

	while(!outCB(curr) && steps >0)
	{
		if(this->at(curr).empty())
			ret_val.push_back( CGChessMove(at,curr));
		else
		{
			break;
		}
		steps--;
		curr = curr+dir;
	}

	curr = at+dir+CGVec2i(1,0);
	if(!outCB(curr) && !this->at(curr).empty() && this->at(curr).color != color)
		ret_val.push_back( CGChessMove(at,curr));
	curr = at+dir+CGVec2i(-1,0);
	if(!outCB(curr) && !this->at(curr).empty() && this->at(curr).color != color)
		ret_val.push_back( CGChessMove(at,curr));

}
//------------------------------------------------------------------------------
void CGChessBoard::removeCheckMoves(std::vector<CGChessMove> &moves) const
{
	std::vector<CGChessMove> ret_val;
	std::vector<CGChessMove> nextMoves;
	nextMoves.reserve(moves.size());
	ret_val.reserve(moves.size());

	for(const auto& move : moves)
	{

		if(!will_move_in_check(move))
			ret_val.push_back(move);
	}
	moves.swap(ret_val);
}
//------------------------------------------------------------------------------
void CGChessBoard::print() const
{

	printf("    A  B  C  D  E  F  G  H\n");
	printf("  *--+--+--+--+--+--+--+--*\n");
	for(int j = 7 ; j >= 0; j--)
	{
		printf("%d |",j+1);
		for(int i = 0;i<8;i++)
		{
			values[i][j].print();
			if (i<7) {
				printf("|");
			}
		}
		printf("| %d\n",j+1);
		if (j>0) {
			printf("  +--+--+--+--+--+--+--+--+\n");
		}
	}
	printf("  *--+--+--+--+--+--+--+--*\n");
	printf("   A  B  C  D  E  F  G  H\n");

	fflush(stdout);
}
//------------------------------------------------------------------------------
std::vector<CGChessMove> CGChessBoard::getMovesThreateningKing(const CGChessColor color) const
{
	std::vector<CGChessMove> nextMoves;
	// generate a list of all moves, the opponent could do on this board
	this->generatePossibleMoves((CGChessColor)((color+1)%2),nextMoves);
	std::vector<CGChessMove> threateningMoves;
	CGVec2i kings_position = this->find(CG_CHESS_KING,color);
	for(const auto& m:nextMoves)
		if (m.to == kings_position)
		{
			threateningMoves.push_back(m);
		}
	return threateningMoves;
}
//------------------------------------------------------------------------------
void CGChessBoard::generatePossibleMoves(const CGChessColor color,
					 const CGVec2i &at,
					 std::vector<CGChessMove> &ret_val)const
{
	const auto& it = this->at(at);

	if((it.piece == CG_CHESS_EMPTY) || it.color != color)
		return;
	switch (it.piece)
	{
	case CG_CHESS_KING  : generatePossibleMovesKing(color,at,ret_val);break;
	case CG_CHESS_QUEEN : generatePossibleMovesQueen(color,at,ret_val);break;
	case CG_CHESS_ROOK  : generatePossibleMovesRook(color,at,ret_val);break;
	case CG_CHESS_BISHOP: generatePossibleMovesBishop(color,at,ret_val);break;
	case CG_CHESS_KNIGHT: generatePossibleMovesKnight(color,at,ret_val);break;
	case CG_CHESS_PAWN  : generatePossibleMovesPawn(color,at,ret_val);break;
	case CG_CHESS_EMPTY:  break;
	}
}
//------------------------------------------------------------------------------
bool CGChessBoard::will_move_in_check(const CGChessMove& move) const
{
	CGChessBoard nextBoard = *this;
	CGChessColor color = this->at(move.from).color;
	nextBoard.makeMove(move);

	CGVec2i king_pos = nextBoard.find(CG_CHESS_KING,color);
	std::vector<CGChessMove> nextMoves;
	nextBoard.generatePossibleMoves((CGChessColor)((color+1)%2),nextMoves);

	for(const auto& nextMove:nextMoves)
	{
		if(nextMove.to == king_pos)
		{
			return  true;
		}
	}
	return false;

}
//------------------------------------------------------------------------------
bool CGChessBoard::is_valid_move(const CGChessMove &move) const
{
	if(!is_possible_move(move))
		return  false;

	if(will_move_in_check(move))
		return  false;

	return true;
}
//------------------------------------------------------------------------------
bool CGChessBoard::is_possible_move(const CGChessMove &move) const
{
	if(at(move.from).empty())
		return  false;

	std::vector<CGChessMove> moves;
	generatePossibleMoves(at(move.from).color,move.from,moves);

	for(const auto& m : moves)
	{
		if(move.to == m.to)
		{
			return  true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
void CGChessBoard::generatePossibleMoves(const CGChessColor color,
					 std::vector<CGChessMove> &ret_val) const
{
	for(int i = 0; i < 8;i++)
		for(int j = 0; j < 8;j++)
		{
			generatePossibleMoves(color,CGVec2i(i,j),ret_val);
		}
}
//------------------------------------------------------------------------------
