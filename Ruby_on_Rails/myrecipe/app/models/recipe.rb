class Recipe < ActiveRecord::Base
  # Name between 5 to 100 characters
  # Summary between 10 to 150 characters
  # Description between 20 to 1000 characters
  # chef_id must be present

  validates :name, presence: true, length: { minimum: 5, maximum: 100 }
  validates :summary, presence: true, length: { minimum: 10, maximum: 150 }
  validates :description, presence: true, length: { minimum: 20, maximum: 1000 }

end
