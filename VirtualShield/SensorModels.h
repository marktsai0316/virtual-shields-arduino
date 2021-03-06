/*
    Copyright(c) Microsoft Open Technologies, Inc. All rights reserved.

    The MIT License(MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files(the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions :

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef SensorModels_h
#define SensorModels_h

typedef unsigned int UINT;

enum SensorAction
{
	Stop = 0,
	Once = 1,
	Start = 2,
	OnceOnChange = 3
};

enum EPtrType
{
	None = 0,
	ProgPtr = 1,
	MemPtr = 2,
	Int = 3,
	Uint = 4,
	Double = 5,
	Long = 6,
	Bool = 7,
	Char = 8,
	ArrayStart = 9,
	ArrayEnd = 10,
	ValueOnly = 11,
	Format = 12,
	Parse = 13
};

union ARGB
{
	uint32_t color;
	struct
	{
		uint8_t blue, green, red, alpha;
	};

	ARGB(byte alpha, byte red, byte green, byte blue) :
		red(red), green(green), blue(blue), alpha(alpha)
	{
	}

	ARGB(byte red, byte green, byte blue) :
		red(red), green(green), blue(blue), alpha(0)
	{
	}

	ARGB() :color(0) {}

	ARGB(unsigned long color) : color(color) {}

	ARGB(String hex) : ARGB((unsigned long)strtol(&hex[hex[0] == '#'], NULL, 16))
	{
	}

	void hex(char* hexSource)
	{
		char hex[9] =
		{ alpha >> 4, alpha & 0x0F,
		  red >> 4, red & 0x0F,
		  green >> 4, green & 0x0F,
		  blue >> 4, blue & 0x0F };

		for (int i = 0; i < 8; i++)
		{
			hexSource[i] = hex[i] + (hex[i] > 0x09 ? 0x37 : 0x30);
		}

		hexSource[8] = 0;

		return;
	}
};

const bool AsText = true;

struct EPtr
{
	EPtrType ptrType;
	const char* key = 0;
	union
	{
		const char* value = 0;
		double doubleValue;
		uint32_t uintValue;
		int intValue;
		long longValue;
		bool boolValue;
		char charValue;
	};

	int length;
	bool keyIsMem = false;
	bool asText = false;
	bool encoded = false;
	EPtr* eptrs = 0;

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	EPtr() {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="ptrType">Type of the EPtr.</param>
	EPtr(EPtrType ptrType) : ptrType(ptrType) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="ptrType">Type of the EPtr.</param>
	EPtr(EPtrType ptrType, const char* key, EPtr* eptrs, int len) : ptrType(ptrType), key(key), intValue(len), eptrs(eptrs), asText(true) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="ptrType">Type of the EPtr.</param>
	/// <param name="key">The key.</param>
	EPtr(EPtrType ptrType, const char* key) : ptrType(ptrType), key(key) {}
	
	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="ptrType">Type of the PTR.</param>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
	EPtr(EPtrType ptrType, const char* key, const char* value) : ptrType(ptrType), key(key), value(value), asText(true), length(-1) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
	EPtr(const char* key, const char* value) : key(key), value(value), asText(true), ptrType(ProgPtr) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
	EPtr(const char* key, String value) : key(key), asText(true), ptrType(value ? MemPtr : None), length(-1)
	{
		this->value = value.c_str();
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
	EPtr(const char* key, const char value) : key(key), asText(true), ptrType(value ? Char : None), charValue(value) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
	/// <param name="ptrType">Type of the EPtr.</param>
	EPtr(const char* key, int value, EPtrType ptrType = Int) : key(key), intValue(value), ptrType(ptrType) {}
	
	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
	/// <param name="ptrType">Type of the EPtr.</param>
	EPtr(const char* key, uint32_t value, EPtrType ptrType = Uint) : key(key), uintValue(value), ptrType(ptrType) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
	/// <param name="ptrType">Type of the EPtr.</param>
	EPtr(const char* key, long value, EPtrType ptrType = Long) : key(key), longValue(value), ptrType(ptrType) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
	/// <param name="asText">As text.</param>
	EPtr(const char* key, double value, bool asText = false) : key(key), doubleValue(value), asText(asText), ptrType(Double) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
	EPtr(const char* key, bool value) : key(key), boolValue(value), ptrType(Bool) {}

	EPtr(const char* key, const char* value, int length) : key(key), value(value), ptrType(MemPtr), length(length) {}

	static int parse(const char* text, EPtr* eptrs, int length, const char separator = '|', int eptrStartIndex = 0)
	{
		int index = 0;
		int start = 0;
		int count = 0;
		while (text[index] || index>start)
		{
			if (!text[index] || text[index] == separator)
			{
				eptrs[eptrStartIndex++] = EPtr(0, text + start, index - start);
				start = index + 1;

				if (++count == length || !text[index])
				{
					break;
				}
			}

			index = index + 1;
		}

		return count;
	}
};

#endif