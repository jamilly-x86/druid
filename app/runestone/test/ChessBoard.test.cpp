#include <gtest/gtest.h>
#include <ChessBoard.h>
#include <Types.h>

using runestone::ChessBoard;

TEST(ChessBoard, defaultConstructsToEmpty)
{
    constexpr ChessBoard board;

    EXPECT_TRUE(board.is_empty());
    EXPECT_EQ(board.raw(), 0ULL);
    EXPECT_EQ(board.pop_count(), 0);
}

TEST(ChessBoard, constructsFromRawValue)
{
	constexpr auto set_bits = 8;
    constexpr ChessBoard board(runestone::bitmask::AFile.raw());

    EXPECT_EQ(board.raw(), runestone::bitmask::AFile.raw());
    EXPECT_FALSE(board.is_empty());
    EXPECT_EQ(board.pop_count(), set_bits);
}

TEST(ChessBoard, setIndividualBits)
{
	ChessBoard board;
	board.set_square(runestone::chessboard_square::A1);

	EXPECT_TRUE(board.test(runestone::chessboard_square::A1));
	EXPECT_FALSE(board.is_empty());
	EXPECT_EQ(board.pop_count(), 1);

	board.set_square(runestone::chessboard_square::H8);
	EXPECT_TRUE(board.test(runestone::chessboard_square::H8));

	constexpr auto count = 2;
	EXPECT_EQ(board.pop_count(), count);
}

TEST(ChessBoard, clearIndividualBits)
{
	ChessBoard board(runestone::bitmask::AFile.and_(runestone::bitmask::Rank1).raw());
	board.clear_square(runestone::chessboard_square::A1);

	EXPECT_FALSE(board.test(runestone::chessboard_square::A1));
	EXPECT_EQ(board.pop_count(), 0);
}

TEST(ChessBoard, toggleIndividualBits)
{
	ChessBoard board;
    board.toggle_square(runestone::chessboard_square::A2);
	board.toggle_square(runestone::chessboard_square::A3);

    EXPECT_TRUE(board.test(runestone::chessboard_square::A2));
    EXPECT_TRUE(board.test(runestone::chessboard_square::A3));

	constexpr auto count = 2;
    EXPECT_EQ(board.pop_count(), count);
}

TEST(ChessBoard, leastSignificantBitIndex)
{
    constexpr ChessBoard board(runestone::bitmask::AFile.raw());
    EXPECT_EQ(board.least_significant_bit(), 0);
}

TEST(ChessBoard, mostSignificantBitIndex)
{
    constexpr ChessBoard board(runestone::bitmask::AFile.raw());
	constexpr auto set_bit_index = 56;

    EXPECT_EQ(board.most_significant_bit(), set_bit_index);
}

TEST(ChessBoard, popLSBRemovesBitsInOrder)
{
	constexpr auto bits_set2 = 0b10010000ULL;
	constexpr auto bit_index2 = 4;
	constexpr auto bits_set1 = 0b10000000ULL;
	constexpr auto bit_index1 = 7;

    ChessBoard board(bits_set2);

    EXPECT_EQ(board.pop_lsb(), bit_index2);
    EXPECT_EQ(board.raw(), bits_set1);

    EXPECT_EQ(board.pop_lsb(), bit_index1);
    EXPECT_TRUE(board.is_empty());
}

TEST(ChessBoard, orCombinesDisjointBits)
{
    constexpr ChessBoard afile(runestone::bitmask::AFile.raw());
    constexpr ChessBoard bfile(runestone::bitmask::BFile.raw());
    constexpr ChessBoard result = afile.or_(bfile);

    EXPECT_EQ(result.raw(), runestone::bitmask::AFile.raw() | runestone::bitmask::BFile.raw());
	// afile should be unchanged.
    EXPECT_EQ(afile.raw(), runestone::bitmask::AFile.raw());
	// bfile should be unchanged.
    EXPECT_EQ(bfile.raw(), runestone::bitmask::BFile.raw());
}

TEST(ChessBoard, andKeepsCommonBits)
{
	constexpr ChessBoard afile(runestone::bitmask::AFile.raw());
	constexpr ChessBoard bfile(runestone::bitmask::BFile.raw());
	constexpr ChessBoard result = afile.and_(bfile);

	EXPECT_EQ(result.raw(), runestone::bitmask::AFile.raw() & runestone::bitmask::BFile.raw());
	// afile should be unchanged.
	EXPECT_EQ(afile.raw(), runestone::bitmask::AFile.raw());
	// bfile should be unchanged.
	EXPECT_EQ(bfile.raw(), runestone::bitmask::BFile.raw());
}

TEST(ChessBoard, xorTogglesBits)
{
	constexpr ChessBoard afile(runestone::bitmask::AFile.raw());
	constexpr ChessBoard bfile(runestone::bitmask::BFile.raw());
	constexpr ChessBoard result = afile.xor_(bfile);

	EXPECT_EQ(result.raw(), runestone::bitmask::AFile.raw() ^ runestone::bitmask::BFile.raw());
	// afile should be unchanged.
	EXPECT_EQ(afile.raw(), runestone::bitmask::AFile.raw());
	// bfile should be unchanged.
	EXPECT_EQ(bfile.raw(), runestone::bitmask::BFile.raw());
}

TEST(ChessBoard, notInvertsBits)
{
    constexpr ChessBoard afile(runestone::bitmask::AFile.raw());
    constexpr ChessBoard result = afile.not_();

    EXPECT_EQ(result.raw(), ~runestone::bitmask::AFile.raw());
	EXPECT_EQ(afile.raw(), runestone::bitmask::AFile.raw());
}

TEST(ChessBoard, orAssign)
{
    ChessBoard afile(runestone::bitmask::AFile.raw());
    constexpr ChessBoard bfile(runestone::bitmask::BFile.raw());
    afile.or_assign(bfile);

    EXPECT_EQ(afile.raw(), runestone::bitmask::AFile.raw() | runestone::bitmask::BFile.raw());
	EXPECT_EQ(bfile.raw(), runestone::bitmask::BFile.raw());
}

TEST(ChessBoard, andAssign)
{
	ChessBoard afile(runestone::bitmask::AFile.raw());
	constexpr ChessBoard bfile(runestone::bitmask::BFile.raw());
	afile.and_assign(bfile);

	EXPECT_EQ(afile.raw(), runestone::bitmask::AFile.raw() & runestone::bitmask::BFile.raw());
	EXPECT_EQ(bfile.raw(), runestone::bitmask::BFile.raw());
}

TEST(ChessBoard, xorAssign)
{
	ChessBoard afile(runestone::bitmask::AFile.raw());
	constexpr ChessBoard bfile(runestone::bitmask::BFile.raw());
	afile.xor_assign(bfile);

	EXPECT_EQ(afile.raw(), runestone::bitmask::AFile.raw() ^ runestone::bitmask::BFile.raw());
	EXPECT_EQ(bfile.raw(), runestone::bitmask::BFile.raw());
}

TEST(ChessBoard, equalityAndInequalityOperators)
{
    constexpr ChessBoard afile(runestone::bitmask::AFile);
	constexpr ChessBoard afile2(runestone::bitmask::AFile);
    constexpr ChessBoard bfile(runestone::bitmask::BFile);
    constexpr ChessBoard cfile(runestone::bitmask::CFile);

    EXPECT_TRUE(afile.equals(afile2));
    EXPECT_TRUE(afile.not_equals(bfile));
    EXPECT_FALSE(afile.not_equals(afile2));
    EXPECT_FALSE(bfile.equals(cfile));
}
