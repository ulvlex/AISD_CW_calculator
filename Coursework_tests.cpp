#include "pch.h"
#include "CppUnitTest.h"
#include "..\Сoursework\Tokens.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Courseworktests
{
	TEST_CLASS(Courseworktests)
	{
	public:
		
		TEST_METHOD(create_tokens_method)
		{
			token token;
			string expr = "5+4";
			token.create_tokens(expr);
			Assert::IsTrue(token.out() == "5+4");
			token.clear();
			expr = "log(5)+sin(6)+4^(2)";
			token.create_tokens(expr);
			Assert::IsTrue(token.out() == "log(5)+sin(6)+4^(2)");
			token.clear();
		}

		TEST_METHOD(balancing_brackets_method)
		{
			token token;
			string expr = "5+4";
			token.create_tokens(expr);
			Assert::IsTrue(token.balancing_brackets(expr) == true);
			token.clear();
			expr = "log(5)+sin(6)+4^(2)))";
			token.create_tokens(expr);
			Assert::IsTrue(token.balancing_brackets(expr) == false);
			token.clear();
		}

		TEST_METHOD(prefix_form_method)
		{
			token token;
			string expr = "5+4";
			token.create_tokens(expr);
			token.prefix_form();
			Assert::IsTrue(token.out_res() == "+54");
			token.clear();
			expr = "(sin(5)+4)*(10+3)";
			token.create_tokens(expr);
			token.prefix_form();
			Assert::IsTrue(token.out_res() == "*+sin(5)4+103");
			token.clear();
		}

		TEST_METHOD(answer_method)
		{
			token token;
			string expr = "5.6+4.3*(3.6+6)/2";
			token.create_tokens(expr);
			token.prefix_form();
			Assert::IsTrue(token.answer() == 26.24);
			token.clear();
		}

	};
}
