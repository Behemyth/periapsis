import std;
import synodic.periapsis;

int main()
{
	peri::InplaceString<16> value {std::string_view {"consumer"}};
	return value.size() == 8 ? 0 : 1;
}
