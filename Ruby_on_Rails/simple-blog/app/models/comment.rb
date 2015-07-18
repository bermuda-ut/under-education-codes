# Author      : Max Lee
# Email       : hoso1312@gmail.com
# Last Edited : Sun Jul 19 05:19:47 AEST 2015

class Comment < ActiveRecord::Base
  belongs_to :article
end
