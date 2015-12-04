require 'test_helper'

class ChefTest < ActiveSupport::TestCase
  
  def setup
    @chef = Chef.new(name: "Jack", email: "Jack@example.com")
  end

  test "Chef should be valid" do
    assert @chef.valid?
  end

  test "Chef name should be present" do
    @chef.name = ""
    assert !@chef.valid?
  end

  test "Chef name should not be too long" do
    @chef.name = "a"*42
    assert !@chef.valid?
  end

  test "Chef name should not be too short" do
    @chef.name = "a"
    assert !@chef.valid?
  end

  test "Chef email should be present" do
    @chef.email = ""
    assert !@chef.valid?
  end

  test "Chef email must be unique" do
    dup_chef = @chef.dup
    dup_chef.email = @chef.email.upcase
    @chef.save
    assert !dup_chef.valid?
  end

  test "Chef email should not be too long" do
    @chef.email = "a"*51 + "@example.com"
    assert !@chef.valid?
  end

  test "Chef email should not be too short" do
    @chef.email = "a"
    assert !@chef.valid?
  end

  test "Chef email validation should accept valid emails" do
    valids = %w[user@example.com R_TDD=D@eee.hello.org user@example.com flst@example.com.au]
    valids.each do |va|
      @chef.email = va
      assert @chef.valid?, "#{va.inspect} should be valid"
    end
  end

  test "Chef email validation should decline invalid emails" do
    valids = %w[@example.com R_TDD=D user@example@com flst@exam++ple.com.au]
    valids.each do |ia|
      @chef.email = ia
      assert !@chef.valid?, "#{ia.inspect} should not be valid"
    end
  end

end
