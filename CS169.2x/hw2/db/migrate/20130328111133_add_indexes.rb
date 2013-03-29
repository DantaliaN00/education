class AddIndexes < ActiveRecord::Migration
  def change
    add_index :reviews, :movie_id
    add_index :reviews, :moviegoer_id
  end
end
