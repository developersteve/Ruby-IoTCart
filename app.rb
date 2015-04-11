require 'sinatra'
require 'braintree'
require 'awesome_print'

  Braintree::Configuration.environment = :sandbox
  Braintree::Configuration.merchant_id = '23nd25g4kn7gnqbb'
  Braintree::Configuration.public_key = '8552x2ym5bvhsycp'
  Braintree::Configuration.private_key = '17f3279171d4fd90ee9cd5256be17abf'

  get '/' do
    @client_token = Braintree::ClientToken.generate
    erb :index
  end

  post '/proc' do

    if params[:nonce] && params[:amt]

    result = Braintree::Transaction.sale(
      :amount => params[:amt],
      :payment_method_nonce => params[:nonce],
      :custom_fields => {
        :cart => params[:cart]
      }
    )

    if result.success?
      result.transaction.id
    else
      'failed'
    end

  end

  end