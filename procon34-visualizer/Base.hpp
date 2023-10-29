# pragma once
# include <Siv3D.hpp>


struct CELL {
	static const CELL NONE;
	static const CELL POND;
	static const CELL WALL_RED;
	static const CELL WALL_BLUE;
	static const CELL AREA_RED;
	static const CELL AREA_BLUE;
	static const CELL CRAFTSMAN_RED; // 敵のagent
	static const CELL CRAFTSMAN_BLUE; // 味方のagent
	static const CELL CASTLE;
	static const CELL CRAFTSMAN; // CRAFTSMAN_BLUE | Enemey
	static const CELL WALL; // WALL_BLUE | WALL_RED
	static const CELL AREA; // AREA_BLUE | AREA_RED

	inline constexpr CELL(const unsigned char v = (unsigned char)0) : val(v) {}
	inline constexpr CELL& operator|=(const CELL s) noexcept { val |= s.val; return *this; }
	inline constexpr CELL& operator&=(const CELL s) noexcept { val &= s.val; return *this; }
	inline constexpr CELL& operator^=(const CELL s) noexcept { val ^= s.val; return *this; }
	inline constexpr CELL operator|(const CELL s) const noexcept { return CELL(val | s.val); }
	inline constexpr CELL operator&(const CELL s) const noexcept { return CELL(val & s.val); }
	inline constexpr CELL operator^(const CELL s) const noexcept { return CELL(val ^ s.val); }
	inline constexpr CELL operator~() const noexcept { return CELL(~val); }
	inline constexpr operator bool() const noexcept { return val; }
	inline constexpr bool operator==(const CELL s) const noexcept { return val == s.val; }
	friend std::ostream& operator<<(std::ostream& os, const CELL s) { return os << (int)s.val; }

protected:
	unsigned char val;
};

struct TEAM {
	static const TEAM RED;
	static const TEAM BLUE;
	inline constexpr TEAM(const unsigned char v = (unsigned char)0) : val(v) {}
	inline constexpr TEAM operator!() const noexcept { return TEAM(!val); }
	inline constexpr bool operator==(const TEAM t) const noexcept { return val == t.val; }
	inline constexpr operator bool() const noexcept { return val; }
protected:
	unsigned char val;
};

CELL switch_cell(const CELL type, const TEAM team);


Rect draw_button_rect(Rect rect, HSV button_color, const HSV& shadow_color);
void draw_button_label(const String& lable, const Rect& rect, const Font& font, const HSV& color, bool is_center = false);







