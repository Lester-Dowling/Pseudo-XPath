// Pseudo-XPath/Grade.cpp
// Started 14 Apr 2019
#include <stdexcept>

#include "Pseudo-XPath/Grade.hpp"
#include "Pseudo-XPath/mini-grammar.hpp"
namespace pseudo_xpath {
	using std::string;

	std::string Grade::attribute_filters_to_string()
	{
		std::string result;
		if (!attribute_filters.empty()) {
			result += '[';
			bool insert_space = false;
			for (Attribute_Filter const& af : attribute_filters) {
				if (insert_space)
					result += " ";
				insert_space = true;
				result += af.attribute_name;
				result += af.filter_operator;
				if (!af.good_filter_number)
					result += '\'';
				result += af.filter_value;
				if (!af.good_filter_number)
					result += '\'';
			}
			result += ']';
		}
		return result;
	}


	std::string Grade::to_string()
	{
		if (name().empty())
			return {};
		else
			return name() + attribute_filters_to_string() +
				   (next() ? " --> " + next()->to_string() : string{});
	}


	/*static*/ std::string Grade::path_to_string(const SP root)
	{
		if (!root)
			return {};
		else if (!root->next())
			return {};
		else
			return root->next()->to_string();
	}

	/*static*/ Grade::SP Grade::parse(std::string const& xpath_text)
	{
		using std::string;
		using std::runtime_error;
		namespace spirit = boost::spirit;
		namespace qi = spirit::qi;
		namespace ascii = spirit::ascii;
		using String_Iterator = string::const_iterator;
		using Stream_Iterator = boost::spirit::basic_istream_iterator<char>;
		using XPath_Grammar = pseudo_xpath::mini_grammar<String_Iterator>;

		XPath_Grammar xpath_parser;
		String_Iterator sitr = xpath_text.begin();
		String_Iterator const send = xpath_text.end();
		if (!qi::phrase_parse(sitr, send, xpath_parser, ascii::space))
			throw runtime_error{ "Failed to parse this XPath: " + xpath_text };
		return xpath_parser.result;
	}
} // namespace pseudo_xpath
