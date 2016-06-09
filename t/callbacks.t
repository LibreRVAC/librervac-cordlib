# This file is part of the LibreRVAC project
#
# Copyright © 2015-2016
#     Aleks-Daniel Jakimenko-Aleksejev <alex.jakimenko@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

use v6;
use Test;
use NativeCall;

plan 8;

sub set_callback_setup(&callback ()) is native(‘cordlib’, v1) {…}
sub set_callback_step(&callback ())  is native(‘cordlib’, v1) {…}
sub cord_event_init()                is native(‘cordlib’, v1) {…}

my $setup = 0;
set_callback_setup {
    $setup++;
};

my $step = 0;
set_callback_step {
    once {
        ok $setup == 1, ‘setup() was called before step()’;
    }
    $step++;
};

cord_event_init();

ok $setup == 1, ‘setup() was called once’;
ok $step > 1,   ‘step() was called multiple times’;



sub set_callback_beep(&callback (num32, num32)) is native(‘cordlib’, v1) {…}
sub hw_beep(num32 $frequency, num32 $duration)  is native(‘cordlib’, v1) {…}

my $freq = 0;
my $duration = 0;
set_callback_beep {
    $freq += $^a;
    $duration += $^b;
};
hw_beep(5000.5.Num, 0.25.Num);

is-approx $freq, 5000.5, ‘beep() callback works (freq)’;
is-approx $duration, 0.25, ‘beep() callback works (duration)’;



sub set_callback_get_time(&callback ( --> num32)) is native(‘cordlib’, v1) {…}
sub hw_get_time() returns num32                   is native(‘cordlib’, v1) {…}

set_callback_get_time sub {
    return 4567.8.Num;
};
is-approx hw_get_time(), 4567.8.Num, ‘get_time() callback works’;



sub set_callback_set_motor_utilization(&callback (int32, num32))    is native(‘cordlib’, v1) {…}
sub hw_set_motor_utilization(int32 $motor_type, num32 $utilization) is native(‘cordlib’, v1) {…}

my $motor_type = 0;
my $utilization = 0;
set_callback_set_motor_utilization sub {
    $motor_type += $^a;
    $utilization += $^b;
};

hw_set_motor_utilization(2, 0.5.Num);
is $motor_type, 2, ‘set_motor_utilization() callback works (motor type)’;
is-approx $utilization, 0.5.Num, ‘set_motor_utilization() callback works (utilization)’;
