require 'sinatra'
require 'erb'

CLIENT_ID = "499019063517475"
REDIRCT_URI = "http://localhost:4567/success"

CONFIG_DIRECTORY = ENV["HOME"] + "/.fbfs"
TOKEN_FILE = "auth_token"

get '/' do
  redirect "https://www.facebook.com/dialog/oauth?client_id=#{CLIENT_ID}&redirect_uri=#{REDIRCT_URI}&response_type=token&scope=read_stream"
end

get '/success' do
  erb :success
end

get '/access_token' do
  access_token = params['access_token']
  # FIXME: not sure if we'll need this later
  #expires_in = params['expires_in']

  Dir.mkdir(CONFIG_DIRECTORY) unless File.exists?(CONFIG_DIRECTORY)
  File.open(CONFIG_DIRECTORY + "/" + TOKEN_FILE, 'w') {|f| f.write(access_token) }
end
