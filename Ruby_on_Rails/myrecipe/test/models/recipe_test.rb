require 'test_helper'

class RecipeTest < ActiveSupport::TestCase

  def setup
    @chef = Chef.create(name: "Max", email: "max@example.com")
    @recipe = @chef.recipes.build(name: "Chicken Pie",
                                  summary: "Best chicken pie recipe in the website",
                                  description: "Heat oil, add onions, add magic, walah! Adabakadabra!")
  end

  test "Recipe should be valid" do
    assert @recipe.valid?
  end

  test "chef_id must be present" do
    @recipe.chef_id = nil
    assert !@recipe.valid?
  end

  test "Name should be present" do
    @recipe.name = ""
    assert !@recipe.valid?
  end

  test "Name length should not be too long" do
    @recipe.name = "A"*101
    assert !@recipe.valid?
  end

  test "Name length should not be too short" do
    @recipe.name = "A"*4
    assert !@recipe.valid?
  end

  test "Summary must be present" do
    @recipe.summary = "";
    assert !@recipe.valid?
  end

  test "Summary should not be too long" do
    @recipe.summary = "A"*151
    assert !@recipe.valid?
  end

  test "Summary should not be too short" do
    @recipe.summary = "A"*4
    assert !@recipe.valid?
  end

  test "Description must be present" do
    @recipe.description = "";
    assert !@recipe.valid?
  end

  test "Description should not be too long" do
    @recipe.description = "A"*1001
    assert !@recipe.valid?
  end

  test "Desciption should not be too short" do
    @recipe.description = "A"*10
    assert !@recipe.valid?
  end

end
