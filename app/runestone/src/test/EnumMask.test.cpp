#include <EnumMask.h>

#include <gtest/gtest.h>
#include <magic_enum/magic_enum.hpp>

namespace
{
	using runestone::operator|;
	using runestone::operator&;
	using runestone::operator^;
	using runestone::operator~;

	enum class Permissions : uint8_t
	{
		None = 0,
		Read = 1 << 0,	  // 1
		Write = 1 << 1,	  // 2
		Execute = 1 << 2, // 4
		Delete = 1 << 3	  // 8
	};

	constexpr auto Write = magic_enum::enum_integer(Permissions::Write);
	constexpr auto ReadWrite = magic_enum::enum_integer(Permissions::Read) + magic_enum::enum_integer(Permissions::Write);
	constexpr auto ReadExecute = magic_enum::enum_integer(Permissions::Read) + magic_enum::enum_integer(Permissions::Execute);
	constexpr auto ReadWriteExecute =
		magic_enum::enum_integer(Permissions::Read) + magic_enum::enum_integer(Permissions::Write) + magic_enum::enum_integer(Permissions::Execute);

	enum class FileFlags : uint8_t
	{
		Hidden = 1 << 0,
		ReadOnly = 1 << 1,
		System = 1 << 2
	};

	// For testing with different underlying types
	enum class ByteEnum : uint8_t
	{
		Flag1 = 1,
		Flag2 = 2,
		Flag3 = 4
	};

	enum class LongEnum : uint64_t
	{
		Flag1 = 1ULL << 0,
		Flag2 = 1ULL << 32,
		Flag3 = 1ULL << 63
	};
}

TEST(EnumMask, defaultConstruction)
{
	constexpr runestone::EnumMask<Permissions> mask;

	EXPECT_EQ(mask.raw(), 0);
	EXPECT_FALSE(mask.test(Permissions::Read));
	EXPECT_FALSE(mask.test(Permissions::Write));
}

TEST(EnumMask, constructFromEnum)
{
	constexpr runestone::EnumMask mask(Permissions::Read);

	EXPECT_EQ(mask.raw(), 1);
	EXPECT_TRUE(mask.test(Permissions::Read));
	EXPECT_FALSE(mask.test(Permissions::Write));
}

TEST(EnumMask, constructFromUnderlyingType)
{
	constexpr runestone::EnumMask<Permissions> mask(ReadExecute);

	EXPECT_EQ(mask.raw(), ReadExecute);
	EXPECT_TRUE(mask.test(Permissions::Read));
	EXPECT_FALSE(mask.test(Permissions::Write));
	EXPECT_TRUE(mask.test(Permissions::Execute));
}

TEST(EnumMask, constructFromMultipleFlags)
{
	constexpr auto mask = Permissions::Read | Permissions::Write;

	EXPECT_EQ(mask.raw(), ReadWrite);
	EXPECT_TRUE(mask.test(Permissions::Read));
	EXPECT_TRUE(mask.test(Permissions::Write));
	EXPECT_FALSE(mask.test(Permissions::Execute));
}

TEST(EnumMask, bitwiseOrWithEnum)
{
	constexpr runestone::EnumMask mask(Permissions::Read);
	constexpr auto result = mask | Permissions::Write;

	EXPECT_EQ(result.raw(), ReadWrite);
	EXPECT_TRUE(result.test(Permissions::Read));
	EXPECT_TRUE(result.test(Permissions::Write));

	// original should be unchanged
	EXPECT_EQ(mask.raw(), 1);
}

TEST(EnumMask, bitwiseOrWithMask)
{
	constexpr runestone::EnumMask mask1(Permissions::Read);
	constexpr runestone::EnumMask mask2(Permissions::Write);
	constexpr auto result = mask1 | mask2;

	EXPECT_EQ(result.raw(), ReadWrite);
	EXPECT_TRUE(result.test(Permissions::Read));
	EXPECT_TRUE(result.test(Permissions::Write));
}

TEST(EnumMask, bitwiseOrEnumToEnum)
{
	constexpr auto result = Permissions::Read | Permissions::Write;

	EXPECT_EQ(result.raw(), 3);
	EXPECT_TRUE(result.test(Permissions::Read));
	EXPECT_TRUE(result.test(Permissions::Write));
}

TEST(EnumMask, bitwiseOrChaining)
{
	constexpr auto result = Permissions::Read | Permissions::Write | Permissions::Execute;

	EXPECT_EQ(result.raw(), ReadWriteExecute);
	EXPECT_TRUE(result.test(Permissions::Read));
	EXPECT_TRUE(result.test(Permissions::Write));
	EXPECT_TRUE(result.test(Permissions::Execute));
}

TEST(EnumMask, compoundOrAssignmentEnum)
{
	runestone::EnumMask mask(Permissions::Read);
	mask |= Permissions::Write;

	EXPECT_EQ(mask.raw(), ReadWrite);
	EXPECT_TRUE(mask.test(Permissions::Read));
	EXPECT_TRUE(mask.test(Permissions::Write));
}

TEST(EnumMask, compoundOrAssignmentChaining)
{
	runestone::EnumMask mask(Permissions::Read);
	mask |= Permissions::Write;
	mask |= Permissions::Execute;

	EXPECT_EQ(mask.raw(), ReadWriteExecute);
	EXPECT_TRUE(mask.test(Permissions::Read));
	EXPECT_TRUE(mask.test(Permissions::Write));
	EXPECT_TRUE(mask.test(Permissions::Execute));
}

TEST(EnumMask, bitwiseAndWithEnum)
{
	constexpr auto mask = Permissions::Read | Permissions::Write | Permissions::Execute;
	constexpr auto result = mask & Permissions::Read;

	EXPECT_EQ(result.raw(), 1);
	EXPECT_TRUE(result.test(Permissions::Read));
	EXPECT_FALSE(result.test(Permissions::Write));
}

TEST(EnumMask, bitwiseAndWithMask)
{
	constexpr auto mask1 = Permissions::Read | Permissions::Write;
	constexpr auto mask2 = Permissions::Write | Permissions::Execute;
	constexpr auto result = mask1 & mask2;

	EXPECT_EQ(result.raw(), Write);
	EXPECT_FALSE(result.test(Permissions::Read));
	EXPECT_TRUE(result.test(Permissions::Write));
	EXPECT_FALSE(result.test(Permissions::Execute));
}

TEST(EnumMask, bitwiseAndEnumToEnum)
{
	constexpr auto result = Permissions::Read & Permissions::Write;

	EXPECT_EQ(result.raw(), 0);
}

TEST(EnumMask, compoundAndAssignmentEnum)
{
	auto mask = Permissions::Read | Permissions::Write | Permissions::Execute;
	mask &= Permissions::Read;

	EXPECT_EQ(mask.raw(), 1);
	EXPECT_TRUE(mask.test(Permissions::Read));
	EXPECT_FALSE(mask.test(Permissions::Write));
}

// TEST(EnumMask, bitwiseAndRemoveFlag)
// {
// 	auto mask = Permissions::Read | Permissions::Write;
// 	mask &= ~Permissions::Write;
//
// 	EXPECT_EQ(mask.raw(), 1);
// 	EXPECT_TRUE(mask.test(Permissions::Read));
// 	EXPECT_FALSE(mask.test(Permissions::Write));
// }

TEST(EnumMask, bitwiseXorWithEnum)
{
	constexpr runestone::EnumMask mask(Permissions::Read);
	constexpr auto result = mask ^ Permissions::Write;

	EXPECT_EQ(result.raw(), ReadWrite);
	EXPECT_TRUE(result.test(Permissions::Read));
	EXPECT_TRUE(result.test(Permissions::Write));
}

TEST(EnumMask, bitwiseXorToggle)
{
	constexpr auto mask = Permissions::Read | Permissions::Write;
	constexpr auto result = mask ^ Permissions::Read;

	EXPECT_EQ(result.raw(), Write);
	EXPECT_FALSE(result.test(Permissions::Read));
	EXPECT_TRUE(result.test(Permissions::Write));
}

TEST(EnumMask, bitwiseXorWithMask)
{
	constexpr auto mask1 = Permissions::Read | Permissions::Write;
	constexpr auto mask2 = Permissions::Write | Permissions::Execute;
	constexpr auto result = mask1 ^ mask2;

	EXPECT_EQ(result.raw(), ReadExecute);
	EXPECT_TRUE(result.test(Permissions::Read));
	EXPECT_FALSE(result.test(Permissions::Write));
	EXPECT_TRUE(result.test(Permissions::Execute));
}

TEST(EnumMask, bitwiseXorEnumToEnum)
{
	constexpr auto result = Permissions::Read ^ Permissions::Write;

	EXPECT_EQ(result.raw(), ReadWrite);
}

TEST(EnumMask, compoundXorAssignmentEnum)
{
	runestone::EnumMask mask(Permissions::Read);
	mask ^= Permissions::Write;

	EXPECT_EQ(mask.raw(), ReadWrite);
	EXPECT_TRUE(mask.test(Permissions::Read));
	EXPECT_TRUE(mask.test(Permissions::Write));
}

TEST(EnumMask, compoundXorToggleTwice)
{
	runestone::EnumMask mask(Permissions::Read);
	mask ^= Permissions::Write;
	mask ^= Permissions::Write;

	EXPECT_EQ(mask.raw(), 1);
	EXPECT_TRUE(mask.test(Permissions::Read));
	EXPECT_FALSE(mask.test(Permissions::Write));
}

TEST(EnumMask, bitwiseNotMask)
{
	constexpr runestone::EnumMask mask(Permissions::Read);
	constexpr auto result = ~mask;

	// all bits except read should be set
	EXPECT_FALSE(result.test(Permissions::Read));
	EXPECT_TRUE(result.test(Permissions::Write));
	EXPECT_TRUE(result.test(Permissions::Execute));
	EXPECT_TRUE(result.test(Permissions::Delete));
}

TEST(EnumMask, bitwiseNotEnum)
{
	constexpr auto result = ~Permissions::Read;

	EXPECT_FALSE(result.test(Permissions::Read));
	EXPECT_TRUE(result.test(Permissions::Write));
}

TEST(EnumMask, doubleNegation)
{
	constexpr runestone::EnumMask mask(Permissions::Read);
	constexpr auto result = ~~mask;

	EXPECT_EQ(result.raw(), mask.raw());
}

TEST(EnumMask, testSingleFlag)
{
	constexpr auto mask = Permissions::Read | Permissions::Execute;

	EXPECT_TRUE(mask.test(Permissions::Read));
	EXPECT_FALSE(mask.test(Permissions::Write));
	EXPECT_TRUE(mask.test(Permissions::Execute));
	EXPECT_FALSE(mask.test(Permissions::Delete));
}

TEST(EnumMask, testNoneFlag)
{
	constexpr runestone::EnumMask mask(Permissions::None);

	EXPECT_FALSE(mask.test(Permissions::Read));
	EXPECT_FALSE(mask.test(Permissions::Write));
	EXPECT_FALSE(mask.test(Permissions::Execute));
	EXPECT_FALSE(mask.test(Permissions::Delete));
}

TEST(EnumMask, testAllFlags)
{
	constexpr auto mask = Permissions::Read | Permissions::Write | Permissions::Execute | Permissions::Delete;

	EXPECT_TRUE(mask.test(Permissions::Read));
	EXPECT_TRUE(mask.test(Permissions::Write));
	EXPECT_TRUE(mask.test(Permissions::Execute));
	EXPECT_TRUE(mask.test(Permissions::Delete));
}