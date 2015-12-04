class Chef < ActiveRecord::Base
  VALID_EMAIL_REGEX = /\A([\w+\-].?)+@[a-z\d\-]+(\.[a-z]+)*\.[a-z]+\z/i
  before_save { self.email = email.downcase }

  validates :name, presence: true, length: {minimum: 3, maximum: 40}
  validates :email, presence: true, length: {minimum: 3, maximum: 50}, 
            uniqueness: { case_sensitive: false }, format: {with: VALID_EMAIL_REGEX}

end

