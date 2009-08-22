-- test.lua: tests for lua-noise library
-- Copyright (c) 2009 lua-noise authors
-- See copyright information in the COPYRIGHT file

-- TODO: Those are merely smoke tests. Write a full test suite!

local print = print

package.cpath = package.cpath .. './?.so'

perlin = require('perlin')

math.randomseed(1250924760)

local p = perlin.new()

for i = 1, 1e6 do
  print(p:noise1(1 / i))
end
