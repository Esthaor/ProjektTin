-- Created by Grzegorz Majchrzak

protocolSIDnet = Proto("SIDnet","SIDnet Protocol")

-- funkcja do rozlozenia pakietow
function protocolSIDnet.dissector(buff ,pinfo, tree)
	local len = buff:len()
	local dataTVBrange = buff:range(0,len)
	local dataString = dataTVBrange:string()
	
	local index1
	local index2
	
	index1, index2 = string.find(dataString, "{\"status\"")
	if (index1 ~= nil) then
		pinfo.cols.protocol = "SIDnet"
		pinfo.cols.info = "Komunikat: "
		local pierwszy = 1
		local subtree = tree:add(protocolSIDnet, buff(), "SIDnet Data")
		for word in string.gmatch(dataString, '([^,]+)') do
			if (pierwszy ~= 1) then
				pinfo.cols.info:append(", ")
			end		
			pierwszy = 0
			local typ = nil
			local wartosc = nil
			for word2 in string.gmatch(word, '([^:]+)') do
				if (typ ~= nil) then
					wartosc = word2
					wartosc = wartosc:gsub("\"", "")
					wartosc = wartosc:gsub("{", "")
					wartosc = wartosc:gsub("}", "")
					wartosc = wartosc:gsub(" ", "")
				else
					typ = word2
					typ = typ:gsub("\"", "")
					typ = typ:gsub("{", "")
					typ = typ:gsub("}", "")
					typ = typ:gsub(" ", "")
				end
			end
			subtree:add(typ,": ", wartosc)
			pinfo.cols.info:append(typ .. " = " .. wartosc)
		end
	end
end

tcp_table = DissectorTable.get("tcp.port")

tcp_table:add(5000,protocolSIDnet)
tcp_table:add(5010,protocolSIDnet)


