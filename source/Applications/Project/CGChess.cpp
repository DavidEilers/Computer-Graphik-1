#include "CGChess.h"


const CGChessBoard &CGChessSystem::board() const
{
	return  m_board;
}
//------------------------------------------------------------------------------
CGChessColor CGChessSystem::toMove() const
{
	return m_to_move;
}
//------------------------------------------------------------------------------
bool CGChessSystem::makeMove(const CGChessMove &move)
{
	if(m_board.at(move.from).color != m_to_move)
		return false;

	if(!m_board.is_valid_move(move))
		return false;

	m_board.makeMove(move);
	m_to_move = (CGChessColor) ((m_to_move+1)%2);
	return true;
}
//------------------------------------------------------------------------------
bool CGChessSystem::testMove(const CGChessMove &move) const
{
	if (move.from.x < 0 || move.from.x > 7 || move.from.y < 0 || move.from.y > 7 || move.to.x > 7 || move.to.y > 7)
		return false;
	if(m_board.at(move.from).color != m_to_move)
		return false;

	if(!m_board.is_valid_move(move))
		return false;

	return true;
}

//------------------------------------------------------------------------------
CGChessMove CGChessSystem::getAIMove()
{
	const auto ret_val = m_ai.getAMove(m_board,m_to_move);
	return  ret_val;
}
//------------------------------------------------------------------------------
void CGChessSystem::resetToTestCase()
{
	m_board.resetToTestCase();
	m_to_move = CG_CHESS_WHITE;
}
//------------------------------------------------------------------------------
#include "CGChessdata.h"
static const float* chess_normals[] = {king_nrm,queen_nrm,rook_nrm,bishop_nrm,knight_nrm,pawn_nrm,NULL};
static const float* chess_positions[] = {king_pos,queen_pos,rook_pos,bishop_pos,knight_pos,pawn_pos,NULL};
static const CGPrimitiveType chess_primitives[] = {king_prim,queen_prim,rook_prim,bishop_prim,knight_prim,pawn_prim,CG_POINTS};
static const unsigned int chess_num_verts[] = {king_num_verts,queen_num_verts,rook_num_verts,bishop_num_verts,knight_num_verts,pawn_num_verts,0};
//------------------------------------------------------------------------------
const float *CGChessSystem::getNormals(const CGChessPieceID cp) const
{
	return chess_normals[cp];
}
//------------------------------------------------------------------------------
const float *CGChessSystem::getPositions(const CGChessPieceID cp) const
{
	return chess_positions[cp];
}
//------------------------------------------------------------------------------
unsigned int CGChessSystem::getNumVerts(const CGChessPieceID cp) const
{
	return chess_num_verts[cp];
}
//------------------------------------------------------------------------------
CGPrimitiveType CGChessSystem::getPrimitve(const CGChessPieceID cp) const
{
	return chess_primitives[cp];
}
//------------------------------------------------------------------------------
