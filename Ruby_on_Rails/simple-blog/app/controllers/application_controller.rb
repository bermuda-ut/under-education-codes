# Author      : Max Lee
# Email       : hoso1312@gmail.com
# Last Edited : Sun Jul 19 05:19:47 AEST 2015

class ApplicationController < ActionController::Base
  # Prevent CSRF attacks by raising an exception.
  # For APIs, you may want to use :null_session instead.
  protect_from_forgery with: :exception
end
