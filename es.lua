local clock = os.clock

function sleep(n)  -- seconds
  local t0 = clock()
  while clock() - t0 <= n do end
end


rec = addBox(400, 400, 100, 100, GREEN)
--circle = addCircle(400, 400, 10, RED)
--onClickCircle(circle, function() io.write("LLLLLUA CLECKD\n") end)
onClickBox(rec, function() io.write("LLLLLUA CLECKD\n") end)

io.write("Sleeping..\n")
sleep(5)
io.write("Deleting..\n")
--deleteCircle(circle)
deleteBox(rec)
