/*
 * oled.hpp
 *
 *  Created on: 20 Sep 2019
 *      Author: dwd
 *
 * This class models the SH1106 oled Display driver.
 */

#pragma once
#include "spi.h"
#include "oled/common.hpp"

#include <map>
#include <functional>

class SS1106 : public SpiInterface  {

	static const std::map<ss1106::Operator, uint8_t> opcode;

	struct Command
	{
		ss1106::Operator op;
		uint8_t payload;
	};

	enum class Mode : uint_fast8_t
	{
		normal,
		second_arg
	} mode = Mode::normal;

	ss1106::State* state;

	Command last_cmd = Command{ss1106::Operator::NOP, 0};

	std::function<bool()> getDCPin;


	uint8_t mask(ss1106::Operator op);
	Command match(uint8_t cmd);

public:
	SS1106(std::function<bool()> getDCPin);
	~SS1106();

	uint8_t write(uint8_t byte) override;
};
