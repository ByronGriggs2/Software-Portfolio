extends Node

func getTextWidth(rich_text_label: RichTextLabel, text: String) -> float:
	var font: Font = rich_text_label.get_theme_font("normal_font") # or custom font
	var font_size: int = rich_text_label.get_theme_font_size("normal_font_size")
	return font.get_string_size(text, font_size).x
