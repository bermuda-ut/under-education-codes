class AddChefIdToRecipe < ActiveRecord::Migration
  def change
    add_column :recipes, :chef_id, :integer
  end
end
