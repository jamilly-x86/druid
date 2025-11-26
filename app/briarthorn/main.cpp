
import druid.core.enummask;

enum class Type : int
{
    One,
    Two,
    Three
};

auto main() -> int
{
    druid::core::EnumMask<Type> test{};
    test.set(Type::One);
    return 0;
}