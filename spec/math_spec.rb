#  Copyright (C) 2009 by Cies Breijs
#  Copyright (C) 2009 by Niels Slot
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public
#  License as published by the Free Software Foundation; either
#  version 2 of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public
#  License along with this program; if not, write to the Free
#  Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
#  Boston, MA 02110-1301, USA.

require File.dirname(__FILE__) + '/spec_helper.rb'
$i = Interpreter.instance

describe "mathmatical operators" do

  it "should work as expected with basic operations" do
    $i.should_run_clean <<-EOS
      assert 1 + 1 == 2
      assert 1 - 1 == 0
      assert 1 - 2 == -1
      assert 1 - -2 == 3
      assert -1 + 1 == 0
      assert 2 * 2 == 4
      assert -2 * 2 == -4
      assert -2 * -2 == 4
      assert 5 / 5 == 1
      assert -5 / 5 == -1
      assert 1 / 1 == 1
      assert 0 / 1 == 0
      assert -0 / 1 == 0
      assert -0 / -1 == 0
      assert 2 ^ 1 == 2
      assert 2 ^ 2 == 4
      assert -2 ^ 2 == 4
      assert 2 ^ -2 == 0.25
      assert 2 ^ 0 == 1
      assert -3 ^ 3 == -27
      assert (sqrt 1) == 1
      assert (sqrt 0) == 0
      assert (sqrt 4) == 2
      assert ((sqrt 2) > 1.4) and ((sqrt 2) < 1.5)
      assert (round 1.5) == 2
      assert (round 1.4) == 1
      assert (round -1.5) == -2
      assert (round -1.4) == -1
    EOS
  end

  it "should produce error when deviding by zero" do
    $i.run('$x = 1 / 0').errors?.should be_true
  end

  it "should produce error when taking the sqrt of a negative number " do
    $i.run('$x = sqrt -2').errors?.should be_true
  end

  it "should generate random number within the range" do
    $i.should_run_clean <<-EOS
      assert (random 0, 1) >= 0
      assert (random 0, 1) >= 0
      assert (random 0, 1) >= 0
      assert (random 0, 1) >= 0
      assert (random 0, 1) >= 0
      assert (random 0, 1) >= 0
      assert (random 1, 0) >= 0
      assert (random 1, 0) >= 0
      assert (random 1, 0) >= 0
      assert (random 1, 0) >= 0
      assert (random 1, 0) >= 0

      assert (random 0, 1) <= 1
      assert (random 0, 1) <= 1
      assert (random 0, 1) <= 1
      assert (random 0, 1) <= 1
      assert (random 0, 1) <= 1
      assert (random 0, 1) <= 1
      assert (random 1, 0) <= 1
      assert (random 1, 0) <= 1
      assert (random 1, 0) <= 1
      assert (random 1, 0) <= 1
      assert (random 1, 0) <= 1

      assert (random 0, -1) <= 0
      assert (random 0, -1) <= 0
      assert (random 0, -1) <= 0
      assert (random 0, -1) <= 0
      assert (random 0, -1) <= 0
      assert (random -1, 0) <= 0
      assert (random -1, 0) <= 0
      assert (random -1, 0) <= 0
      assert (random -1, 0) <= 0
      assert (random -1, 0) <= 0

      assert (random 0, -1) >= -1
      assert (random 0, -1) >= -1
      assert (random 0, -1) >= -1
      assert (random 0, -1) >= -1
      assert (random 0, -1) >= -1
      assert (random 0, -1) >= -1
      assert (random -1, 0) >= -1
      assert (random -1, 0) >= -1
      assert (random -1, 0) >= -1
      assert (random -1, 0) >= -1
      assert (random -1, 0) >= -1
    EOS
  end

  it "should work as expected with trigoniometical functions" do
    $i.should_run_clean <<-EOS
      assert (sin -90) == -1
      assert (sin   0) == 0
      assert (sin  90) == 1
      assert (sin 180) == 0
      assert (sin 270) == -1
      assert (sin 450) == 0

      assert (cos -90) == 0
      assert (cos   0) == -1
      assert (cos  90) == 0
      assert (cos 180) == 1
      assert (cos 270) == 0
      assert (cos 450) == -1

      # assert (tan -90)  # division by zero
      assert (tan   0) == 0
      # assert (tan 90)   # division by zero
      assert (tan 180) == 0
      # assert (tan 270)  # division by zero
      assert (tan 450) == 0
    EOS
    $i.run('tan -90').errors?.should be_true
    $i.run('tan 90 ').errors?.should be_true
    $i.run('tan 270').errors?.should be_true

    $i.should_run_clean <<-EOS
      asset arcsin (arcsin  -1) == -90
      asset arcsin (arcsin -.5) == -30
      asset arcsin (arcsin   0) == 0
      asset arcsin (arcsin  .5) == 30
      asset arcsin (arcsin   1) == 90

      asset arcsin (arccos  -1) == 180
      asset arcsin (arccos -.5) == 120
      asset arcsin (arccos   0) == 90
      asset arcsin (arccos  .5) == 60
      asset arcsin (arccos   1) == 0

      asset arctan (arccos  -1) == -45
      asset arctan (arccos   0) == 0
      asset arctan (arccos   1) == 45
      asset arctan (arccos -100000000000) == -90
      asset arctan (arccos  100000000000) == 90
    EOS
    $i.run('sin -1.1').errors?.should be_true
    $i.run('sin  1.1').errors?.should be_true
    $i.run('cos -1.1').errors?.should be_true
    $i.run('cos  1.1').errors?.should be_true
  end

  it "should obey precedence rules"
  it "should work with parentheses"
  it "should work for floating point numbers"
  it "should allow mixing of natural and floating point numbers"

end