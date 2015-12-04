class Recipe < ActiveRecord::Base
  # Name between 5 to 100 characters
  # Summary between 10 to 150 characters
  # Description between 20 to 1000 characters
  # chef_id must be present

  belongs_to :chef

  validates :name, presence: true, length: { minimum: 5, maximum: 100 }
  validates :summary, presence: true, length: { minimum: 10, maximum: 150 }
  validates :description, presence: true, length: { minimum: 20, maximum: 1000 }
  validates :chef_id, presence: true

  mount_uploader :picture, PictureUploader
  validate :picture_size


  private
    def picture_size
      if picture.size > 5.megabytes
        errors.add(pcture, "should be less than 5MB")
      end
    end

end
