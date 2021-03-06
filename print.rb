require 'rubygems'  # allows for the loading of gems
require 'graphviz'  # this loads the ruby-graphviz gem

# Verify the file if all cases are close ----------------
def verif f
	@@tab_node = {}
	@@con = false
	@rel = false
	@con_f = false
	@rel_f = false
	@name_f = false
	begin
		file = File.open f, 'r'
	rescue
		puts 'Error with file: ' + f
		return false
	end

	file.each { |line|
		if line.match 'END' and line[0] == 91
			@name_f = false
			@con_f = false
			@rel_f = false
		end

		if @rel_f 
			l = line.split ' '
			puts " #{l[0]} #{l[1]} #{l[2]} "
			if @@tab_node[l[0].upcase] == nil 
				@@tab_node[l[0].upcase] = 'from 0'
			else 
				@@tab_node[l[0].upcase] = 'both' if @@tab_node[l[0].upcase] == 'to 1'
			end

			if @@tab_node[l[1].upcase] == nil 
				@@tab_node[l[1].upcase] = 'to 1'
			else 
			  @@tab_node[l[1].upcase] = 'both'	if @@tab_node[l[1].upcase] == 'from 0'
			end
		end

		if line.match 'CONS' and line[0] == 91
			if @rel_f == false and @name_f == false 
				@con_f = true 
				@@con = true
			else
				return false
			end
		end

		if line.match 'RELATION' and line[0] == 91
			if @con_f == false and @name_f == false 
				@rel_f = true 
				@rel = true
			else
				return false
			end
		end

		if line.match 'NAME' and line[0] == 91
			if @rel_f == false and @con_f == false 
				@name_f = true 
			else
				return false
			end
		end
	}
	puts	"1: #{@@con} #{@rel} #{@con_f} #{@rel_f} #{@name_f} #{@@tab_node}"
	return @rel
end

# Rename the extention of the file ----------------------
def rename name
	n = name.split '.'
	return n[0].concat('.png') if n.size == 1
	n[n.size-1] = 'png'
  name = n[0]
	(1..n.size-1).each do |i|
		name+= '.' + n[i]
	end
	return name
end

# Read the file and add edges to the graph -----------------
# http://www.graphviz.org/doc/info/colors.html
# , 'decorate'=>'true' 
def add_edge f
	@rel_f = false
	begin
		file = File.open(f,'r')
	rescue
		puts 'Error on file: ' + f
		return false
	end

	case @@con
	when true
		file.each { |line|
			@rel_f = false if line.match 'END' and line[0] == 91
			if @rel_f
				l = line.split ' '
				if @@consos.key? "#{l[0].upcase} -> #{l[1].upcase}"
					label = @@consos.fetch("#{l[0].upcase} -> #{l[1].upcase}").to_s + "/" + l[2]
					color = ( @@consos.fetch("#{l[0].upcase} -> #{l[1].upcase}").to_f / l[2].to_f ) * 100.0
					puts @@consos.fetch("#{l[0].upcase} -> #{l[1].upcase}") + "  #{color}"					
					case color.to_i
					when 0..13 then color_nbr = 11
					when 14..25 then color_nbr = 10
					when 26..37 then color_nbr = 9
					when 38..49 then color_nbr = 8
					when 50..61 then color_nbr = 4
					when 62..73 then color_nbr = 3
					when 74..85 then color_nbr = 2
					when 86..100 then color_nbr = 1
					end
				else
					label = "0/" + l[2]
				end
				if color_nbr 
					@g.add_edge(l[0].upcase, l[1].upcase, "colorscheme" => 'rdylgn11', 'color' => color_nbr.to_s) .label= label
				else 
					@g.add_edge(l[0].upcase, l[1].upcase, 'colorscheme' => 'rdylgn11', "color" => '11').label= label
				end
			end
			@rel_f = true if line.match 'RELATION' and line[0] == 91 
		}

	when false		
		file.each { |line|
			@rel_f = false if line.match 'END' and line[0] == 91
			if @rel_f
				l = line.split ' '
				@g.add_edge(l[0].upcase, l[1].upcase ).label= l[2]
			end
			@rel_f = true if line.match 'RELATION' and line[0] == 91 		
		}
	end
	return true
end

# Make the graph ------------------------------
def graph f
	# initialize new Graphviz graph
	@g = GraphViz::new( "structs", "type" => "graph" )
	@g[:rankdir] = 'LR'
	@g[:colorscheme] = 'ylorrd9'
	@g.edge[:arrowsize]= "0.5"
	@g.edge[:arrowhead]= "open"
	@@tab_node.each { |key, val|			
			@g.add_node(key,"shape" => "doublecircle" ) unless val == 'both'
			@g.add_node(key) if val == 'both'
	}
	@g.output( "output" => "png", :file => rename(f) ) if add_edge f 
end

# Load the consumtion if there are -------------
def load_con f
	file = File.open(f,'r')
	@@consos = {}
	@con = false
	file.each do |line|
		@con = false if line.match 'END' and line[0] == 91
		if @con and line[0] != 35
			l = line.split ' ' 
			@@consos["#{l[0].upcase} -> #{l[1].upcase}"] = l[2]
		end
		@con = true if line.match 'CONS' and line[0] == 91
	end
	file.close
	puts "2: #{@@consos} "
end

# Do it for all the file in argument --------------
ARGV.each do |file|
	if verif(file)
		load_con(file)	if @@con
		graph(file)
	else 
		puts 'Error with file: ' + file
	end
end
