<?php

require 'Braintree.php';

Braintree_Configuration::environment('sandbox');
Braintree_Configuration::merchantId('23nd25g4kn7gnqbb');
Braintree_Configuration::publicKey('8552x2ym5bvhsycp');
Braintree_Configuration::privateKey('17f3279171d4fd90ee9cd5256be17abf');

$now = new Datetime();
$past = clone $now;
$past = $past->modify("-1 days");

class MyDB extends SQLite3
{
        function __construct()
        {
                $this->open('/www/proc/YunPHP/print.db');
        }
}

$db = new MyDB();

$lookup = Braintree_Transaction::search(array(
        Braintree_CustomerSearch::createdAt()->between($past, $now),
));

foreach ($lookup->_ids as $ids) {

        $record = Braintree_Transaction::find($ids);

        if (!file_exists("/www/proc/YunPHP/files/" . $ids) and $record->customFi

                $fh = fopen("/www/proc/YunPHP/files/" . $ids, 'w');

                $cart = json_decode($record->customFields['cart'], true);

                fwrite($fh, "Order #" . $ids . "\n\n");

                foreach ($cart as $key => $value) {
                        if (array_key_exists("total", $value)) {
                                fwrite($fh, "Total: $" . $value["total"]);
                        } else {
                                fwrite($fh, $value['qty'] . " " . $value["itemName"] . "\n");
                                fwrite($fh, "$" . $value["price"] . "\n\n");
                        }
                }

                fclose($fh);

                $db->exec("INSERT INTO print (trans, ind) VALUES ('".$ids."', 0)");

    }

}

?>