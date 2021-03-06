#!/usr/bin/ruby

$KCODE = 'u'
require 'jcode'
require 'rubygems'
require 'couchrest_model'
require 'active_record'

def create_class(class_name, superclass, &block)
  klass = Class.new superclass, &block
  cn = class_name.split(/::/)
  if cn.size == 1
    m = Object
    o = cn.first
  elsif cn.size == 2
    m = Module.const_get(cn.first)
    o = cn.last
  else
    raise "WTF #{class_name}"
  end
  m.const_set o, klass
end

db_name = 'kikkanji_test2'
ActiveRecord::Base.establish_connection(:adapter => 'sqlite3', :database => '/Users/somekool/Projects/kikkanji/contents/code/kanji.kexi')

# note that views might be proceed along with tables as well, depending on your adapter. but it's a good thing. once you rewrite your views into couchDB code, you will be able to compare the output with those "backups"

module SRC; end
module DST; end
tables = ActiveRecord::Base.connection.tables
tables.each do |t|
  puts "Starting with table: #{t}"
  class_name = t.gsub(/_+/, '_').singularize.classify
  src_class_name = "SRC::#{class_name}"
  dst_class_name = "DST::#{class_name}"
  src_class = create_class(src_class_name, ActiveRecord::Base) {set_table_name t.to_sym}
  dst_class = create_class(dst_class_name, CouchRest::Model::Base)
  dst_class.send :use_database, db_name
  dst_class.send 'timestamps!'.to_sym
  src_class.columns.each { |c|
    puts "#{c.name} - #{c.type.to_s.classify}"
    options = {}
    options[:default] = c.default if c.default
    options[:limit] = c.limit if c.limit
    options[:null] = c.null if c.null
    if c.type.to_s.empty?
      if "Byte UNSIGNED" == c.sql_type
        column_type = Integer
      else
        raise "WTF #{c.inspect}"
      end
    elsif c.type == :binary
      column_type = String
    elsif c.type == :text
      column_type = String
    else
      column_type = Object.const_get(c.type.to_s.classify)
    end
    dst_class.send :property, c.name.to_sym, column_type
  }
  
  src_class.find(:all).each { |row| 
    o = dst_class.new(row.attributes)
    o.save
  }
end




