------------------------------------------------------------------------
------------------------------------------------------------------------
--- Author: Pujan Dave                                               ---
---         B.S., Computer Engineering                               ---
---         University of Illinois at Urbana-Champaign               ---
---         Social Robotics Lab, NUS                                 ---
------------------------------------------------------------------------
------------------------------------------------------------------------
---                        Requires torch7                           ---
---                                                                  ---
---  This code calls the actual program that would                   ---
---   detect the emotions.                                           ---
---                                                                  ---
---  To run this code:                                               ---
---     th createDataSet.lua                                         ---
------------------------------------------------------------------------
------------------------------------------------------------------------

require 'sys'
require 'os'

-- Local definitions -----------------------------------------------------------
local pf = function(...) print(string.format(...)) end
local Cn = sys.COLORS.none
local CC = sys.COLORS.Cyan
local CG = sys.COLORS.Green

author = [[

 _____
/  __ `    _
| |__) |  |_|
|  ___, _  _  __ _ _ __
| | |  | || |/ _` | `  `
| | \__/ || | (_| | || |
\_/\___,_|| |\__,_|_||_|
         _' '
        |__/ 
]]

thanks = [[

 _____ _               _     _   _
|_   _| |             | | __| | | |
  | | | |__  __ _ _ __| |/ /| | | |
  | | | ,, \/ _` | `  `   / | | | |
  | | | || | (_| | || | \ \ | |_| |
  \_/ \_/\_/\__,_|_||_|_|\_\\_____/
]]

pf(CC .. author .. Cn)
os.execute('python /home/sunknight/FaceEmotions/faceProcessing/detectEmotions.py')
pf(CG .. thanks .. Cn)

