require 'rubygems'  # allows for the loading of gems
require 'graphviz'  # this loads the ruby-graphviz gem


def verif file
	return true
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

	file.each do |line|
		@rel = false if line.match 'END' and line[0] != 35
		if @rel 
			l = line.split ' '
			g.add_edge(l[0].upcase, l[1].upcase ).label= l[2]
		end
		@rel = true if line.match 'RELATION' and line[0] != 35
	end
	g.output( "output" => "png", :file => rename(f) )
	file.close
end

ARGV.each do |file|
	graph(file) if verif(file)
end
