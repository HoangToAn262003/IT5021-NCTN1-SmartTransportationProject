//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package gr1_w6_veins_gridtest2.veins_inet;
//import org.car2x.veins.modules.application.ieee80211p.DemoBaseApplLayer;
import org.car2x.veins.modules.application.traci.TraCIDemo11p;

simple VehicleControlApp extends TraCIDemo11p
{
    @class(veins::VehicleControlApp);
    string appName = default("My first Veins App!");
}