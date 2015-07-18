# Author      : Max Lee
# Email       : hoso1312@gmail.com
# Last Edited : Sun Jul 19 05:19:47 AEST 2015

class Article < ActiveRecord::Base
  has_many :comments, dependent: :destroy
  validates :title, presence: true,
                    length: { minimum: 5 }
end
