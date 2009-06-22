require 'rubygems'  # allows for the loading of gems
require 'graphviz'  # this loads the ruby-graphviz gem


def verif f
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
		if line.match 'END' and line[0] == 91
			@name_f = false
			@con_f = false
			@rel_f = false
		end
	}
	puts	"1: #{@@con} #{@rel} #{@con_f} #{@rel_f} #{@name_f}"
	return @rel
end

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

def graph f
	begin
		file = File.open(f,'r')
	rescue
		puts 'Error on file: '+f
		return 
	end
	# initialize new Graphviz graph
	g = GraphViz::new( "structs", "type" => "graph" )
	g[:rankdir] = "LR"
	g.edge[:arrowsize]= "0.5"
	g.edge[:arrowhead]= "open"

	@rel_f = false
	file.each_line do |line|
		@rel_f = false if line.match 'END' and line[0] == 91
		if @rel_f and line[0] != 35 			
			l = line.split ' '
			if @@consos.key? "#{l[0].upcase} -> #{l[1].upcase}"
				label = @@consos.fetch("#{l[0].upcase} -> #{l[1].upcase}").to_s + "/" + l[2]
			else
				label = "0/" + l[2]
			end
			puts "1:#{} 2:#{} lab:#{label}!"
			g.add_edge(l[0].upcase, l[1].upcase ).label= label
		end
		@rel_f = true if line.match 'RELATION' and line[0] == 91
	end
	f = rename f 
	puts "file: #{f}"
	g.output( "output" => "png", :file => rename(f) )
	file.close
end

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

ARGV.each do |file|
	if verif(file)
		load_con(file) if @@con
		graph(file)
	else 
		puts 'Error with file: ' + file
	end
end
