#include "CGChessAI.h"


CGChessMove CGChessAI::getAMove(const CGChessBoard &board, const CGChessColor color)
{
	int currentValue = evaluateBoard(board);
	if(color == CG_CHESS_WHITE)
		Max(board,maxDepth,std::numeric_limits<float>::lowest(),
		    std::numeric_limits<float>::max(), currentValue);
	else
		Min(board,maxDepth,std::numeric_limits<float>::lowest(),
		    std::numeric_limits<float>::max(), currentValue);

	if(nextMove.from[0] <0)
	{
		if(!board.is_draw())
			nextMove= CGChessMove(board.find(CG_CHESS_KING,color),CGVec2i(-1,-1));
		else
			nextMove= CGChessMove(CGVec2i(-1,-1),CGVec2i(-1,-1));
	}
	return nextMove;
}
//------------------------------------------------------------------------------
float CGChessAI::Max(const CGChessBoard &board, unsigned int depth, float alpha, float beta, int currentValue)
{
	if(depth < 1) return evaluateBoard(board,currentValue,depth);

	float maxValue = -std::numeric_limits<float>::max();

	std::vector<CGChessMove> moves;

	board.generatePossibleMoves(CG_CHESS_WHITE,moves);
	board.removeCheckMoves(moves);

	std::vector<CGChessMove> possibleMoves;

	for(const auto& move : moves)
	{

		CGChessBoard nextBoard = board;
		nextBoard.makeMove(move);
		float value = Min(nextBoard, depth-1, alpha, beta, currentValue);

		if (value > maxValue)
		{
			maxValue = value;
			possibleMoves.clear();
		}


		if(value == maxValue)
		{
			possibleMoves.push_back(move);
		}
		if (maxValue > alpha) {
			alpha = value;
		}
		if (alpha >= beta)
			break;

	}

	if(depth == maxDepth)
	{
		if(possibleMoves.empty())
		{
			nextMove= CGChessMove(CGVec2i(-1,-1),CGVec2i(-1,-1));
		}
		else
			nextMove = possibleMoves[(size_t)rand() % possibleMoves.size()];
	}

	return maxValue;
}
//------------------------------------------------------------------------------
float CGChessAI::Min(const CGChessBoard &board, unsigned int depth, float alpha, float beta, int currentValue)
{
	if(depth < 1) return evaluateBoard(board,currentValue, depth);

	float minValue = std::numeric_limits<float>::max();

	std::vector<CGChessMove> moves;
	board.generatePossibleMoves(CG_CHESS_BLACK,moves);
	board.removeCheckMoves(moves);

	std::vector<CGChessMove> possibleMoves;

	for(const auto& move : moves)
	{

		CGChessBoard nextBoard = board;
		nextBoard.makeMove(move);
		float value = Max(nextBoard, depth-1, alpha, beta, currentValue);

		if (value < minValue)
		{
			minValue = value;
			possibleMoves.clear();
		}
		if(value == minValue)
		{
			possibleMoves.push_back(move);
		}
		if (minValue < beta) {
			beta = value;
		}

		if (beta <= alpha)
			break;

	}

	if(depth == maxDepth)
	{
		if(possibleMoves.empty())
		{
			nextMove= CGChessMove(CGVec2i(-1,-1),CGVec2i(-1,-1));
		}
		else
			nextMove = possibleMoves[(size_t)rand() % possibleMoves.size()];
	}

	return minValue;
}
//------------------------------------------------------------------------------
int CGChessAI::evaluateBoard(const CGChessBoard &board)
{
	int value = 0;

	for(int y = 0; y < 8; y++)
		for(int x = 0; x < 8; x++)
		{
			const auto& it = board.at(x,y);
			value += it.value;
		}
	return value;
}
//------------------------------------------------------------------------------
float CGChessAI::evaluateBoard(const CGChessBoard &board, int currentValue, int depth)
{
	const int deltaValue = evaluateBoard(board)-currentValue;
	const float weight = 5.0f/(float)((maxDepth - depth)+5);
	return (float)deltaValue * weight;
}
//------------------------------------------------------------------------------
