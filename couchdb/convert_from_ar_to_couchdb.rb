$KCODE = 'u'
require 'jcode'
require 'active_record'
require 'couchrest_model'

def create_class(class_name, superclass, &block)
  klass = Class.new superclass, &block
  Object.const_set class_name, klass
end

create_class('Person', ActiveRecord::Base) do
  set_table_name :people

  def fullname
    "#{firstname} #{lastname}" # assuming the people table has firstname,lastname columns
  end
end

ActiveRecord::Base.establish_connection(:adapter => 'sqlite3', :database => package.path + '/Users/somekool/Projects/kikkanji/contents/code/kanji.kexi')
tables = ActiveRecord::Base.connection.tables

# note that views might be proceed along with tables as well, depending on your adapter. but it's a good thing. once you rewrite your views into couchDB code, you will be able to compare the output with those "backups"



