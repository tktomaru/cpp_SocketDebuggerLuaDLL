---------------------------------------------
-- 接続完了通知
---------------------------------------------
-- iniフォルダ
function globalIniPath()
   return "D:\\github\\SocketDebuggerLuaDLL\\LuaSample\\"
end
-- 送信フォルダ
function globalSendPath()
   return "D:\\github\\SocketDebuggerLuaDLL\\LuaSample\\sendFiles\\"
end

-- デバッグ用の再起表示関数
function saiki( ret )
    if ( type(ret)=="number" ) then
       Logput(1,ret)
    elseif (type(ret)=="string" ) then
       Logput(1,ret)
    else
      if ret[0] ~= nil then
         Logput(1,'s0')
         saiki( ret[0] )
      end
      if ret[1] ~= nil then
         Logput(1,'s1')
         saiki( ret[1] )
      end
      if ret[2] ~= nil then
         Logput(1,'s2')
         saiki( ret[2] )
      end
      if ret[3] ~= nil then
         Logput(1,'s3')
         saiki( ret[3] )
      end
      if ret[4] ~= nil then
         Logput(1,'s4')
         saiki( ret[4] )
      end
    end
end

function writeCount( sendFileName, count )
   local dd = {}

   dd[0] = "0";    dd[1] = "1"
   dd[2] = "2";    dd[3] = "3"
   dd[4] = "4";    dd[5] = "5"
   dd[6] = "6";    dd[7] = "7"
   dd[8] = "8";    dd[9] = "9"

   local out = ""
   mod = math.floor(count % 10)
   out = out .. dd[mod]
   count = math.floor(count / 10)
   while(count>0) do
      mod = math.floor(count % 10)
      out = dd[mod] .. out
      count =  math.floor(count / 10)
   end

   FileWrite( out , globalSendPath() .. sendFileName .. ".count" ) 
end

function readCount( sendFileName )
   countFileBuf = FileRead(globalSendPath() .. sendFileName .. ".count" )
   local ret = 0

   dd = {}
   dd[48] = 0;    dd[49] = 1;
   dd[50] = 2;    dd[51] = 3;
   dd[52] = 4;    dd[53] = 5;
   dd[54] = 6;    dd[55] = 7;
   dd[56] = 8;    dd[57] = 9;
   if ( type(countFileBuf)=="number" or type(countFileBuf)=="nil" ) then
      Logput(3,"countFileBuf Open Error : " .. countFileBuf)
      FileWrite( "0" , globalSendPath() .. sendFileName .. ".count" ) 
      return 0 
   else
      for ii=1, #countFileBuf do
         ret = ret + dd[countFileBuf[ii]]*(10^(#countFileBuf-ii))
      end
   end
   return ret
end

function OnConnected()
   Logput(1,'OnConnected')
   iniFile = getIniFile(globalIniPath() .. "setting.ini")
   -- iniファイルが正常に開けたか？
   if ( type(iniFile)=="number" ) then
      Logput(3,"IniFile Open Error : " .. iniFile)
   else
   for ii=1, #iniFile do
      filename = iniFile[ii]["filename"]
      --Logput(1, "  filename " .. filename )
      --Logput(1, "  help     " .. help )
      --Logput(1, "  count    " .. count )
      sendFileBuff = FileRead(globalSendPath() .. filename .. ".count" )
      -- .countファイルを作成する（0で初期化）
      FileWrite( "0" , globalSendPath() .. filename .. ".count" )
   end -- for
   end -- if else
   return 0
end
---------------------------------------------
-- 送信ﾎﾞﾀﾝ押下
---------------------------------------------
function OnSendPush()
    Logput(1,'OnSendPush')
    --a = GetEditorData()
    --SendData(a)
    local ini = getIniFile(globalIniPath() .. "setting.ini")
    
    -- デバッグ用にiniファイルの中身を表示
    if ( type(ini)=="number" ) then
       Logput(3,ini)
    else
    for ii=1, #ini do
       Logput(1, "  filename " .. ini[ii]["filename"] )
       Logput(1, "  help     " .. ini[ii]["help"] )
       Logput(1, "  count    " .. ini[ii]["count"] )
       Logput(1, "  maxcount " .. ini[ii]["maxcount"] )
       for jj=1, #ini[ii]["value"] do
       Logput(1, "  start    " .. ini[ii]["value"][jj]["start"] )
       Logput(1, "  length   " .. ini[ii]["value"][jj]["length"] )
       Logput(1, "  value    " .. ini[ii]["value"][jj]["value"] )
       end
    end
    end
    return 0
end
---------------------------------------------
-- ﾀｲﾏｰ通知
---------------------------------------------
function OnTimer(id)
    Logput(1,'OnTimer')
    return 0
end

---------------------------------------------
-- 2byteをintegerに変換する
---------------------------------------------
function read_i16(b1, b2)
  local mask = bitshl(1, 15)
  local res  = bitor( (bitshl(b1, 8)) , (bitshl(b2, 0)) )
  return bitxor(res , mask) - mask
end

function read_ui16(b1, b2)
  local res  = bitor( (bitshl(b1, 8)) , (bitshl(b2, 0)) )
  return res
end
---------------------------------------------
-- numberを length byteに変換する(lengh=4まで対応)
---------------------------------------------
function read_Nbyte(num, length)
  high1 = bitshr( bitand(num, 0xff000000), 8*3)
  high2 = bitshr( bitand(num, 0xff0000),   8*2)
  low1  = bitshr( bitand(num, 0xff00) ,    8)
  low2  = bitand( num, 0xff)

  if (length == 1) then
     return low2
  end
  if (length == 2) then
    return (low1), (low2)
  end
  if (length == 3) then
    return high2,low1, low2
  end
  if (length == 4) then
    return high1, high2,low1, low2
  end
  return 0
end
---------------------------------------------
-- 受信通知
---------------------------------------------
function OnReceive(recv)
    Logput(1,'OnReceive')
    if( #recv > 6 ) then
       --int = read_ui16( recv[1], recv[2] )
       --Logput(1,int)
       local countmax = 0
       ini = getIniFile(globalIniPath() .. "setting.ini")
    
       if ( type(ini)=="number" ) then
          Logput(3,"IniFile Open Error : " .. ini)
       else
       skipii = 0
       for ii=1, #ini do
       -- count内のコマンドの内１つのみ送信するようにスキップする
       if( ii >= skipii ) then
          local filename = ini[ii]["filename"]
          local help = ini[ii]["help"]
          local count = ini[ii]["count"]
          local maxcount = ini[ii]["maxcount"]
          --Logput(1, "   filename " .. filename )
          --Logput(1, "   help     " .. help )
          --Logput(1, "   count    " .. count )
          --Logput(1, "   maxcount    " .. maxcount )
          sendFileBuf = FileRead(globalSendPath() .. filename)
          rcount = readCount(filename)
          matchnum = 0
          -- 送信ファイルのオープンに成功したか？
          if ( type(sendFileBuf)=="number" ) then
             Logput(3, "sendFileBuf Open Error (" .. sendFileBuf.. "): "  .. filename)
          else
          if ( count == rcount ) then
          Logput(1, filename .. ".count=" .. count )
          for jj=1, #ini[ii]["value"] do

             start  = ini[ii]["value"][jj]["start"]
             length = ini[ii]["value"][jj]["length"]
             value  = ini[ii]["value"][jj]["value"]
             --Logput(1, "   start    " .. start )
             --Logput(1, "   length   " .. length )
             --Logput(1, "   value    " .. value )
             -- 受信電文は解析長より大きいか
             if( #recv > start + length) then
                hl = 0
                isfail = 0
                -- Logput(1, "  value    " .. value )
                -- 解析のvalueと電文が一致するか
                for dummy,pv in ipairs({read_Nbyte( value, length )}) do
                  --Logput(1, "  pv    " .. pv )
                  if( recv[start + hl] ~= pv ) then
                     isfail = 1
                     break
                  end
                  hl = hl + 1
                end -- for
                if( isfail == 0 ) then
                   -- 一部の検索値が一致した
                   Logput(1,"   match line=" .. ii .. "  num=" .. jj)
                   matchnum = matchnum + 1
                end -- if
             end -- if
          end  -- for
          end -- if count == rcount

          if( matchnum == #ini[ii]["value"]+1 ) then
             -- すべての検索値が一致した
             Logput(1,"   match command = " .. help)
             if( rcount < maxcount ) then
                writeCount(filename, rcount+1)
             end
             skipii = ii + (maxcount - rcount + 1)
             --Logput(1,"   skipii = " .. skipii)
          end -- if
          end -- number
          end -- if( ii < skipii ) then
          end -- for
       end -- if else
    end
    return 0
end
---------------------------------------------
-- 切断通知
---------------------------------------------
function OnDisConnected()
    Logput(1,'OnDisConnected')
    return 0
end

