Rails.application.routes.draw do
  root 'pages#home'

  #get '/home', to: 'pages#home'
  #get '/recipes', to: 'recipes#index'
  #get '/recipes/:id', to: 'recipes#show', as: 'recipe'
  #get '/recipes/new', to: 'recipes#new', as: 'new_recipe'
  #post '/recipes', to: 'recipes#create'
  #get '/recipes/:id/edit', to: 'recipe#edit', as: 'edit_recipe'
  #patch '/recipes/:id', to: 'recipes#update'
  #delete '/recipes/:id', to: 'recipes#destroy'

  resources :recipes

end

