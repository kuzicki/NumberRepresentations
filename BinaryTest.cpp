#include "pch.h"
#include "CppUnitTest.h"
#include "BinaryNumbers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BinaryTest
{
	TEST_CLASS(BinaryTest)
	{
	public:
		
		TEST_METHOD(DirectBinaryTest)
		{
			DirectBinary db(10);
			int expected = 10;
			Assert::AreEqual(expected, db.Base10());
		}

		TEST_METHOD(InverseBinaryTest)
		{
			InverseBinary ib(10);
			int expected = 10;
			Assert::AreEqual(expected, ib.Base10());
		}

		TEST_METHOD(ComplementBinaryTest)
		{
			ComplementBinary cb(10);
			int expected = 10;
			Assert::AreEqual(expected, cb.Base10());
		}

		TEST_METHOD(MinusDirectBinaryTest)
		{
			DirectBinary db(-10);
			int expected = -10;
			Assert::AreEqual(expected, db.Base10());
		}

		TEST_METHOD(MinusInverseBinaryTest)
		{
			InverseBinary ib(-10);
			int expected = -10;
			Assert::AreEqual(expected, ib.Base10());
		}

		TEST_METHOD(MinusComplementBinaryTest)
		{
			ComplementBinary cb(-10);
			int expected = -10;
			Assert::AreEqual(expected, cb.Base10());
		}

		TEST_METHOD(MinusDirectBinaryBitsTest)
		{
			DirectBinary db(-10);
			std::string expected = "10000000000000000000000000001010";
			Assert::AreEqual(expected, db.ToString());
		}

		TEST_METHOD(InverseBinaryBitsTest)
		{
			InverseBinary ib(10);
			std::string expected = "00000000000000000000000000001010";
			Assert::AreEqual(expected, ib.ToString());
		}

		TEST_METHOD(MinusComplementBinaryBitsTest)
		{
			ComplementBinary cb(-10);
			std::string expected = "11111111111111111111111111110110";
			Assert::AreEqual(expected, cb.ToString());
		}

		TEST_METHOD(ComplementaryAddition) {
			ComplementBinary cb1(10);
			ComplementBinary cb2(2);
			int expected = 12;
			Assert::AreEqual(expected, (cb1 + cb2).Base10());
		}

		TEST_METHOD(ComplementarySubtraction) {
			ComplementBinary cb1(10);
			ComplementBinary cb2(2);
			int expected = 8;
			Assert::AreEqual(expected, (cb1 - cb2).Base10());
		}

		TEST_METHOD(DirectBinaryMultiplication) {
			DirectBinary db1(10);
			DirectBinary db2(2);

			int expected = 20;
			Assert::AreEqual(expected, (db1 * db2).Base10());
		}

		TEST_METHOD(FixedDirectBinaryResultTest) {
			FixedDirectBinary fb1(10);
			FixedDirectBinary fb2(21);

			std::string expected = "0.47619";
			Assert::AreEqual(expected, (fb1 / fb2).Base10());
		}

		TEST_METHOD(MinusFixedDirectBinaryResultTest) {
			FixedDirectBinary fb1(10);
			FixedDirectBinary fb2(-21);

			std::string expected = "-0.47619";
			Assert::AreEqual(expected, (fb1 / fb2).Base10());
		}

		TEST_METHOD(MinusFixedDirectBinaryBitsTest) {
			FixedDirectBinary fb1(10);
			FixedDirectBinary fb2(-21);

			std::string expected = "10000000000000001011101000000011";
			Assert::AreEqual(expected, (fb1 / fb2).ToString());
		}

		TEST_METHOD(FloatingPointAdditionBits) {
			FloatingPoint fp1(10.1f);
			FloatingPoint fp2(21.3f);

			std::string expected = "01000001111110110011001100110011";
			Assert::AreEqual(expected, (fp1 + fp2).ToString());
		}

		TEST_METHOD(MinusFloatingPointAdditionBits) {
			FloatingPoint fp1(10.1f);
			FloatingPoint fp2(-21.3f);

			std::string expected = "11000001001100110011001100110010";
			Assert::AreEqual(expected, (fp1 + fp2).ToString());
		}

		TEST_METHOD(MinusFloatingPointAdditionNumber) {
			FloatingPoint fp1(10.1f);
			FloatingPoint fp2(-21.3f);

			float expected = 11.2f;
			float epsilon = 0.001f;
			Assert::IsTrue((fp1 + fp2).Base10() + expected < epsilon);
		}

		TEST_METHOD(MinusFloatingPointAdditionNumber2) {
			FloatingPoint fp1(-10.f);
			FloatingPoint fp2(0.90f);

			FloatingPoint fres = fp1 + fp2;
			float expected = -9.1f;
			float epsilon = 0.001f;
			Assert::IsTrue((fp1 + fp2).Base10() + expected < epsilon);
		}
	};
}
