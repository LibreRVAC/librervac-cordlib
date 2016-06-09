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

plan 6;

sub set_callback_set_motor_utilization(&callback (int, num32))    is native(‘cordlib’, v1) {…}
sub hw_set_motor_utilization(int32 $motor_type, num32 $utilization) is native(‘cordlib’, v1) {…}
sub cord_set_speed_goal(num32 $speed)                             is native(‘cordlib’, v1) {…}
sub cord_event_bumper(num32 $angle, num32 $uncertainty)           is native(‘cordlib’, v1) {…}

my %utilization is default(0.Num);

set_callback_set_motor_utilization sub {
    %utilization{$^motor_type} = $^utilization; # TODO do not use $^
};

my \term:<WHEEL←> = 0;
my \term:<WHEEL→> = 1;

is %utilization{WHEEL←}, 0, ‘left wheel not moving’;
is %utilization{WHEEL→}, 0, ‘right wheel not moving’;

cord_set_speed_goal(10.Num); # max (in ㎧)

isnt %utilization{WHEEL←}, 0, ‘left wheel activated’;
isnt %utilization{WHEEL→}, 0, ‘right wheel activated’;

cord_event_bumper(0.Num, 15.Num);

is %utilization{WHEEL←}, 0, ‘left wheel stopped after bumper event’;
is %utilization{WHEEL→}, 0, ‘right wheel stopped after bumper event’;

cord_set_speed_goal(10.Num); # max (in ㎧)

# skip ‘Sensors not yet implemented’, 4;

# isnt %utilization{WHEEL←}, 0, ‘left wheel activated again’;
# isnt %utilization{WHEEL→}, 0, ‘right wheel activated again’;

# is %utilization{WHEEL←}, 0, ‘left wheel stopped after cliff event’;
# is %utilization{WHEEL→}, 0, ‘right wheel stopped after cliff event’;
