------------------------------------------------------------------------
------------------------------------------------------------------------
--- Author: Pujan Dave                                               ---
---         B.S., Computer Engineering                               ---
---         University of Illinois at Urbana-Champaign               ---
---         Social Robotics Lab, NUS                                 ---
---                                                                  ---
------------------------------------------------------------------------
------------------------------------------------------------------------

require 'torch'   -- torch
require 'image'   -- to visualize the dataset
require 'nn'      -- provides all sorts of trainable modules/layers

torch.setdefaulttensortype('torch.FloatTensor')

local network = {} -- network object
local source = {}
network.model = torch.load('/home/sunknight/FaceEmotions/torch_module/results/model.net')

src = image.load("/home/sunknight/FaceEmotions/torch_module/input/test.png",1)
img = image.scale(src, 32, 32):clone()
kl = torch.Tensor(1,1,32,32)
kl[1] = img
output = network.model:forward(kl)
maxVal = torch.max(output)

idx = 1

while (output[1][idx] ~= maxVal) do
	idx=idx+1
end

local file,err = io.open("/home/sunknight/FaceEmotions/torch_module/output/class_label.bin", "wb")
file:write(idx-1)
file:close()
